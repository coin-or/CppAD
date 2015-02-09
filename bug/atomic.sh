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
cat << EOF > eigen_plugin.h
typedef Scalar value_type;
EOF
cat << EOF > eigen_sparse_plugin.h
# ifndef CPPAD_EIGEN_SPARSE_PLUGIN_INCLUDED
# define CPPAD_EIGEN_SPARSE_PLUGIN_INCLUDED

typedef Scalar value_type;

# endif

template<typename S, int R, int Opt, int MR, int MC>
inline void reserve(const Matrix<S,R,Opt,MR,MC> &reserveSizes)
{
  reserveInnerVectors(reserveSizes);
}
EOF
cat << EOF > atomic.cpp
#define EIGEN_MATRIX_PLUGIN "$builddir/eigen_plugin.h"
#define EIGEN_SPARSEMATRIX_PLUGIN "$builddir/eigen_sparse_plugin.h"

#include <Eigen/Eigen>
#include <Eigen/Sparse>
#include <cppad/cppad.hpp>


using Eigen::Dynamic;
using Eigen::MatrixBase;
using Eigen::Map;
using Eigen::MatrixXd;
using Eigen::VectorXd;

using CppAD::vector;

typedef CppAD::AD<double> AScalar;
typedef Eigen::Matrix<AScalar, Dynamic, 1> VectorXA;
typedef Eigen::Matrix<AScalar, Dynamic, Dynamic> MatrixXA;
typedef Eigen::Triplet<double> TT;

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


class multpow_cl {

  // Function:  f = a*b^c
  // eval returns the function, function+gradient, or function+gradient+hessian
  
 public:
  
  template<typename TX>
    void eval(const MatrixBase<TX>& x,
	      double& f) {
    
    f = x[0]*pow(x[1],x[2]);
  }

  template<typename TX, typename TD>
    void eval(const MatrixBase<TX>& x, 
	      double& f,
	      MatrixBase<TD>& df) {
    
    const double a = x[0];
    const double b = x[1];
    const double c = x[2];
    
    f = a*pow(b, c);
    df[0] = pow(b, c);
    df[1] = a*c*pow(b,c-1);
    df[2] = a*pow(b, c)*log(b);
  }
  
  
  template<typename TX, typename TD, typename TH>
    void eval(const MatrixBase<TX>& x, 
	      double& f,
	      MatrixBase<TD>& df,
	      MatrixBase<TH>& hess) {
    
    const double a = x[0];
    const double b = x[1];
    const double c = x[2];

    f = a*pow(b, c);
    df[0] = pow(b, c);
    df[1] = a*c*pow(b,c-1);
    df[2] = a*pow(b, c)*log(b);
    
    hess(0,0) = 0;
    hess(1,1) = a*c*(c-1)*pow(b, c-2);
    hess(2,2) = a*pow(log(b),2)*pow(b,c);
    hess(0,1) = c*pow(b, c-1);
    hess(1,0) = hess(0,1);
    hess(0,2) = pow(b,c)*log(b);
    hess(2,0) = hess(0,2);
    hess(1,2) = a*(1+c*log(b))*pow(b,c-1);
    hess(2,1) = hess(1,2); 
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
	
    const Map<const MatrixXd> x = MatrixXd::Map(&(tx[0]),p+1,n);
    Map<VectorXd> y = VectorXd::Map(&(ty[0]),p+1);
	
    bool ok = (p <= 2) && (q <= p);
	
    if( vx.size() > 0 ) {
      vy[0] = vx[0];
      for (size_t j=1; j<n; j++) {
	vy[0] = vy[0] || vx[j];
      }
    }
    
    if ((q <= 0) && (p == 0)) {
      double f;
      func->eval(x.row(0),f);
      y[0] = f;      
    }
	
    if ((q <= 1) && (p == 1)) {
      double f;
      VectorXd df(n);
      func->eval(x.row(0),f,df);
      y[0] = f;      
      y[1] = df.dot(x.row(1));    
    }
	
    if ((q <= 2) && (p == 2)) {
      double f;
      VectorXd df(n);
      MatrixXd hess(n,n);
      func->eval(x.row(0), f, df, hess);
      y[0] = f;      
      y[1] = df.dot(x.row(1)); 	
      y[2] = x.row(1) * hess * x.row(1).transpose();    
      y[2] *= 0.5;
      y[2] += df.dot(x.row(2));
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
	
    const Map<const MatrixXd> x = MatrixXd::Map(&(tx[0]), q+1, n);
	
    double f;
    VectorXd df(n);     
    MatrixXd dy(n, q+1);

    bool ok = (q <= 2);
	
    if (q == 0) {
      func->eval(x.row(0), f, df);
      dy.col(0) = df;
    }
    
    if (q >= 1) {
      MatrixXd hess(n,n);
      func->eval(x.row(0), f, df, hess);
      dy.col(0) = df;
      dy.col(1) = hess * x.row(1).transpose();
    }
	
    for (size_t j=0; j<n; j++){
      for (size_t L=0; L <= q; L++) {
	px[j*(q+1)+L] = 0;
	for (size_t k=L; k<=q; k++) {
	  px[j*(q+1)+L] += py[k]*dy(j, k-L);
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
    size_t n = r.size() / q;
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

    // f = (a1*b1^c1 + a2*b2^c2)^2;
    // where ai = Y[3*i], bi = Y[3*i+1], ci = Y[3*i+2]
    
    size_t n = Y.size()/3;
    AScalar res1 = 0.0;
    for (size_t i=0; i<n; i++) {
      res1 += Y[3*i]*pow(Y[3*i+1], Y[3*i+2]);
    }
    return (res1*res1);
  }
};


int main() {

  CppAD::ADFun<double> tape1;
  CppAD::ADFun<double> tape2;
  CppAD::sparse_hessian_work hess_info;

  int nvars = 6;
  VectorXd X(nvars);
  for(size_t i = 0; i < nvars; i++)
     X[i] = double(i+1) / 10.0;

  VectorXd w(1);
  w[0] = 1.0;

  VectorXA f1(1); // to hold result
  VectorXA f2(1); // to hold result
  VectorXA P(nvars);
  for(size_t i = 0; i < nvars; i++)
     P[i] = X[i];

  multpow_test_atomic func1;
  multpow_test_cppad  func2;

  // record tapes  
  CppAD::Independent(P);
  f1[0] = func1.eval(P);
  tape1.Dependent(P, f1);
  tape1.optimize();


  CppAD::Independent(P);
  f2[0] = func2.eval(P);
  tape2.Dependent(P, f2);
  tape2.optimize();

  // compute f, df, d2f

  tape1.Forward(0, X);
  VectorXd hess_sparse1 = tape1.SparseHessian(X, w);
     
  tape2.Forward(0, X);
  VectorXd hess_sparse2 = tape2.SparseHessian(X, w);
  
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

}
EOF
# -----------------------------------------------------------------------------
name='atomic'
echo \
"g++ -I$HOME/prefix/eigen/include -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I$HOME/prefix/eigen/include -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ./$name
then
	echo "OK"
else
	echo "Error"
fi
