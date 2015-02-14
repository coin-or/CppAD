#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
builddir=`pwd`
# -----------------------------------------------------------------------------
cat << EOF > atomic.cpp
#include <cppad/cppad.hpp>

using CppAD::vector;

typedef CppAD::AD<double> AScalar;
typedef vector<double>    VectorXd;
typedef vector<AScalar>   VectorXA;

VectorXd my_column(
	const double* matrix, size_t n_row, size_t n_col, size_t index)
{	VectorXd col(n_row);
	assert( index < n_col );
	for(size_t i = 0; i < n_row; i++)
		col[i] = matrix[ i * n_col + index];
	return col;
}

double my_dot(const VectorXd& x, const VectorXd& y)
{	assert( x.size() == y.size() );
	double dot = 0.0;
	for(size_t i = 0; i < x.size(); i++)
		dot += x[i] * y[i];
	return dot;
}

inline void my_union(
	      std::set<size_t>&         result  ,
	      const std::set<size_t>&   left    ,
	      const std::set<size_t>&   right   )
{
  std::set<size_t> temp;
  std::set_union(
		 left.begin()              ,
		 left.end()                ,
		 right.begin()             ,
		 right.end()               ,
		 std::inserter(temp, temp.begin())
		  );
  result.swap(temp);
}

void multpow_ad(const VectorXA& ax, VectorXA& ay)
{ 
    ay[0] = ax[0]*pow(ax[1],ax[2]);
    return;
}


class multpow_cl {

  // Function:  f = a*b^c
  // eval returns the function, function+gradient, or function+gradient+hessian

 public:

  void eval(const VectorXd& x, double& f) {
    f = x[0]*pow(x[1],x[2]);
  }

  void eval(const VectorXd& x, double& f, VectorXd& df) {

    const double a = x[0];
    const double b = x[1];
    const double c = x[2];

    f = a*pow(b, c);
    df[0] = pow(b, c);
    df[1] = a*c*pow(b,c-1);
    df[2] = a*pow(b, c)*log(b);
  }


  void eval(const VectorXd& x, double& f, VectorXd& df, VectorXd& hess) {

    const double a = x[0];
    const double b = x[1];
    const double c = x[2];

    f = a*pow(b, c);
    df[0] = pow(b, c);
    df[1] = a*c*pow(b,c-1);
    df[2] = a*pow(b, c)*log(b);

    hess[0 * 3 + 0] = 0;
    hess[1 * 3 + 1] = a*c*(c-1)*pow(b, c-2);
    hess[2 * 3 + 2] = a*pow(log(b),2)*pow(b,c);
    hess[0 * 3 + 1] = c*pow(b, c-1);
    hess[1 * 3 + 0] = hess[0 * 3 + 1];
    hess[0 * 3 + 2] = pow(b,c)*log(b);
    hess[2 * 3 + 0] = hess[0 * 3 + 2];
    hess[1 * 3 + 2] = a*(1+c*log(b))*pow(b,c-1);
    hess[2 * 3 + 1] = hess[1 * 3 + 2];
  }
};


template<typename TF>
class mb_atomic : public CppAD::atomic_base<double> {

  // inherits from atomic_base
  // templated so I can just point to different functions, as above
  // this class should NEVER have to be modified for scalar-valued atomics.

 public:
  std::shared_ptr<TF> func;

 mb_atomic(const std::string& name) :
  CppAD::atomic_base<double>(name) {
    func = std::make_shared<TF>();
  }

  ~mb_atomic() {}

 private:

  // ----------------------------------------------------------------------
  // forward mode routine called by CppAD
  virtual bool forward(
		       size_t                p , // lowest order Taylor coeff
		       size_t                q , // highest order Taylor coeff
		       const vector<bool>&   vx , // no idea
		       vector<bool>&         vy , // no idea
		       const vector<double>& tx , // input vector of x
		       vector<double>&       ty   // output vector of y
		       )
  {

    size_t n = tx.size() / (p+1);

    bool ok = (p <= 2) && (q <= p);

    if( vx.size() > 0 ) {
      vy[0] = vx[0];
      for (size_t j=1; j<n; j++) {
	vy[0] = vy[0] || vx[j];
      }
    }

    if ((q <= 0) && (p == 0)) {
      double f;
      VectorXd x_row_0 = my_column(tx.data(), n, q+1, 0);
      func->eval(x_row_0,f);
      ty[0] = f;
    }

    if ((q <= 1) && (p == 1)) {
      double f;
      VectorXd df(n);
      VectorXd x_row_0 = my_column(tx.data(), n, q+1, 0);
      func->eval(x_row_0,f,df);
      ty[0] = f;
      VectorXd x_row_1 = my_column(tx.data(), n, q+1, 1);
      ty[1] = my_dot(df, x_row_1);
    }

    if ((q <= 2) && (p == 2)) {
      double f;
      VectorXd df(n);
      VectorXd hess(n * n);
      VectorXd x_row_0 = my_column(tx.data(), n, q+1, 0);
      func->eval(x_row_0, f, df, hess);
      ty[0] = f;
      VectorXd x_row_1 = my_column(tx.data(), n, q+1, 1);
      ty[1] = my_dot(df, x_row_1);
      ty[2] = 0.0;
      for(size_t i = 0; i < n; i++)
      {   for(size_t j = 0; j < n; j++)
              ty[2] += x_row_1[i] * hess[i * 3 + j] * x_row_1[j];
      }
      ty[2] *= 0.5;
      VectorXd x_row_2 = my_column(tx.data(), n, q+1, 2);
      ty[2] += my_dot(df, x_row_2);
    }

    return ok;
  }

  // ----------------------------------------------------------------------
  // reverse mode routine called by CppAD
  virtual bool reverse(
		       size_t                 q ,
		       const vector<double>&  tx ,
		       const vector<double>&  ty ,
		       vector<double>&        px ,
		       const vector<double>&  py
		       )
  {
    size_t n = tx.size() / (q+1);

    double f;
    VectorXd df(n);
    VectorXd dy( n * (q+1) );

    bool ok = (q < 2);

    VectorXd x_row_0 = my_column(tx.data(), n, q+1, 0);
    func->eval(x_row_0, f, df);
	for(size_t j = 0; j < n; j++)
       dy[ j*(q+1) + 0 ] = df[j];

    if (q >= 1) {
      VectorXd hess(n * n);
      func->eval(x_row_0, f, df, hess);
      VectorXd x_row_1 = my_column(tx.data(), n, q+1, 1);
      VectorXd dy_col_1(n);
      for(size_t i = 0; i < n; i++)
      {   dy_col_1[i] = 0.0;
          for(size_t j = 0; j < n; j++)
             dy_col_1[i] += hess[ i * 3 + j] * x_row_1[j];
      }
	  for(size_t j = 0; j < n; j++)
         dy[ j*(q+1) + 1 ] = dy_col_1[j];

    }

    for (size_t j=0; j<n; j++){
      for (size_t L=0; L <= q; L++) {
	      px[j*(q+1)+L] = 0;
	      for (size_t k=L; k<=q; k++) {
	          px[j*(q+1)+L] += py[k]*dy[ j*(q+1) + k-L];
	      }
      }
    }
    return ok;
  }


  // ----------------------------------------------------------------------
  // forward Jacobian sparsity routine called by CppAD
  virtual bool for_sparse_jac(
			      size_t                             q ,
			      const vector<std::set<size_t> >&   r ,
			      vector<std::set<size_t> >&         s
			      ) {
    size_t n = r.size();
    s[0] = r[0];
    for (size_t i=0; i<n; i++) {
      my_union(s[0], s[0], r[i]);
    }

    return true;
  }


  // ----------------------------------------------------------------------
  // reverse Jacobian sparsity routine called by CppAD
  virtual bool rev_sparse_jac(
			      size_t  p ,
			      const vector< std::set<size_t> >& rt ,
			      vector< std::set<size_t> >& st
			      ) {

    size_t n = st.size();
    for (size_t i=0; i<n; i++) {
      st[i] = rt[0];
    }
    return true;
  }

  // ----------------------------------------------------------------------
  // reverse Hessian sparsity routine called by CppAD
  virtual bool rev_sparse_hes(
			      const vector<bool>&             vx,
			      const vector<bool>&             s,
			      vector<bool>&                   t,
			      size_t                          p ,
			      const vector<std::set<size_t> >&  r ,
			      const vector<std::set<size_t> >&  u ,
			      vector<std::set<size_t> >&  v
			      )
  {
    size_t n = vx.size();
    size_t m = s.size();
    assert(m==1);

    for (size_t i=0; i<n; i++) {
      t[i] = s[0];
      v[i] = u[0];
      if( s[0] ) {
    	for (size_t j = 0; j < n; j++) {
    	  my_union(v[i], v[i], r[j] );
    	}
      }
    }
    return true;
  }
}; // end class


AScalar multpow_atomic(const AScalar& a, const AScalar& b, const AScalar& c) {
  static mb_atomic<multpow_cl> multpow_func("atomic_multpow");
  VectorXA y(1);
  VectorXA x(3);
  x[0] = a;
  x[1] = b;
  x[2] = c;
  multpow_func(x,y); // f = a*b^c
  return(y[0]);
}

struct multpow_test_atomic {
  AScalar eval(const VectorXA& Y) {


    // f = (a1*b1^c1 + a2*b2^c2)^2;
    // where ai = Y[3*i], bi = Y[3*i+1], ci = Y[3*i+2]

    size_t n = Y.size()/3;
    AScalar res1 = 0.0;
    for (size_t i=0; i<n; i++) {
      res1 += multpow_atomic(Y[3*i], Y[3*i+1], Y[3*i+2]);
    }
    return (res1*res1);
  }
};


struct multpow_test_cppad {
  AScalar eval(const VectorXA& Y) {

    VectorXA ax(3), ay(1);
    size_t n = Y.size()/3;
    AScalar res1 = 0.0;
    for (size_t i=0; i<n; i++) {
      ax[0] = Y[3*i + 0];
      ax[1] = Y[3*i + 1];
      ax[2] = Y[3*i + 2];
      multpow_ad(ax, ay);
      res1 += ay[0];
    }
    return (res1*res1);
  }
};

class multpow_test_checkpoint {
public:
   CppAD::checkpoint<double>* multpow;

   multpow_test_checkpoint(void)
   {   VectorXA ax(3), ay(1);
       ax[0] = 1.0; ax[1] = 1.0; ax[2] = 1.0;
       multpow = new CppAD::checkpoint<double>("multpow", multpow_ad, ax, ay);
   }
   ~multpow_test_checkpoint(void)
   {    delete multpow; }

  AScalar eval(const VectorXA& Y) {

    VectorXA ax(3), ay(1);
    size_t n = Y.size()/3;
    AScalar res1 = 0.0;
    for (size_t i=0; i<n; i++) {
      ax[0] = Y[3*i + 0];
      ax[1] = Y[3*i + 1];
      ax[2] = Y[3*i + 2];
      (*multpow)(ax, ay);
      res1 += ay[0];
    }
    return (res1*res1);
  }
};


int main() {

  CppAD::ADFun<double> tape1, tape2, tape3;
  CppAD::sparse_hessian_work hess_info;

  int nvars = 6;
  VectorXd X(nvars);
  for(size_t i = 0; i < nvars; i++)
     X[i] = double(i+1) / 10.0;

  VectorXd w(1);
  w[0] = 1.0;

  VectorXA f(1); // to hold result
  VectorXA P(nvars);
  for(size_t i = 0; i < nvars; i++)
     P[i] = X[i];

  multpow_test_atomic     func1;
  multpow_test_cppad      func2;
  multpow_test_checkpoint func3;

  // record tapes
  CppAD::Independent(P);
  f[0] = func1.eval(P);
  tape1.Dependent(P, f);

  CppAD::Independent(P);
  f[0] = func2.eval(P);
  tape2.Dependent(P, f);


  CppAD::Independent(P);
  f[0] = func3.eval(P);
  tape3.Dependent(P, f);


  tape1.Forward(0, X);
  VectorXd hess_sparse1 = tape1.SparseHessian(X, w);

  tape2.Forward(0, X);
  VectorXd hess_sparse2 = tape2.SparseHessian(X, w);

  tape3.Forward(0, X);
  VectorXd hess_sparse3 = tape3.SparseHessian(X, w);

  std::cout << "mb_atomic:" << std::endl;
  for(size_t i = 0; i < nvars; i++)
  {  for(size_t j = 0; j < nvars; j++)
        std::cout << " " << std::setw(10)
        << hess_sparse1[ i * nvars + j];
     std::cout << std::endl;
  }

  std::cout << "cppad:" << std::endl;
  for(size_t i = 0; i < nvars; i++)
  {  for(size_t j = 0; j < nvars; j++)
        std::cout << " " << std::setw(10)
        << hess_sparse2[ i * nvars + j];
     std::cout << std::endl;
  }

  std::cout << "checkpoint:" << std::endl;
  for(size_t i = 0; i < nvars; i++)
  {  for(size_t j = 0; j < nvars; j++)
        std::cout << " " << std::setw(10)
        << hess_sparse3[ i * nvars + j];
     std::cout << std::endl;
  }

}
EOF
# -----------------------------------------------------------------------------
name='atomic'
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ./$name
then
	echo "OK"
else
	echo "Error"
fi
