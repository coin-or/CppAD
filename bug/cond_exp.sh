#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
Bug report by Michael Braun:

cppad-20141230 error from unknown source
Error detected by false result for
    var_index_ >= NumRes(op_)
at line 572 in the file 
    ../../cppad/local/player.hpp
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>

using CppAD::vector;
typedef CppAD::AD<double> AScalar;


typedef CppAD::vector<double> vecD;
typedef CppAD::vector<AScalar> vecA;


#ifndef M_LN_SQRT_2PI
#define M_LN_SQRT_2PI 0.9189385332
#endif
#ifndef M_LN2
#define M_LN2 0.69314718056
#endif
#ifndef M_SQRT2
#define M_SQRT2 1.41421356237
#endif

AScalar my_erf_pos(const AScalar x) {

  const AScalar p = 0.3275911;
  const AScalar a1 = 0.254829592;
  const AScalar a2 = -0.284496736;
  const AScalar a3 = 1.421413741;
  const AScalar a4 = -1.453152027;
  const AScalar a5 = 1.061405429;
  
  const AScalar t = 1/(1+p*x);

  AScalar res = t*(a1 + t*(a2 + t*(a3 + t*(a4 + t*a5))));
  res = 1 - res * exp(-x*x);
  return(res);
}

AScalar my_erf(const AScalar x) {

  AScalar r1 = my_erf_pos(x);
  AScalar r2 = -my_erf_pos(-x);
  AScalar res = CppAD::CondExpGe(x, AScalar(0), r1, r2);

  return(res);
}

AScalar dnorm_log(const AScalar& z,
		  const AScalar& m,
		  const AScalar& s) {

  AScalar res = -M_LN_SQRT_2PI -log(s) -(z-m)*(z-m)/(2*s*s);
  return(res);
}

AScalar dnormTrunc0_log(const AScalar& z,
			const AScalar& m,
			const AScalar& s) {
  
  AScalar R = erf(-M_SQRT1_2 * m/s);
  AScalar res = dnorm_log(z, m, s) - log(1.0-R) + M_LN2;
  
  return(res);
}

AScalar dnormTrunc0_log2(const AScalar z,
			const AScalar m,
			const AScalar s) {
  
  AScalar R = my_erf(-M_SQRT1_2 * m/s);
  AScalar res = dnorm_log(z, m, s) - log(1.0-R) + M_LN2;
  
  return(res);
}

int main() {

  using std::cout;
  using std::endl;
  
  CppAD::ADFun<double> tape1;
  CppAD::ADFun<double> tape2;

  int nvars = 3;
  vecD X(nvars);
  X[0] = -.1;
  X[1] = .2;
  X[2] = .3;
  
  vecA P(nvars);
  for (size_t i=0; i<nvars; i++) {
    P[i] = AScalar(X[i]);
  }

  vecD w(1);
  w[0] = 1.0;

  vecA f1(1); // to hold result
  vecA f2(1); // to hold result

  cout.precision(3);
  cout.width(5);

  // using CppAD/C++11 erf function
  CppAD::Independent(P);
  AScalar q1 = dnormTrunc0_log(P[0], P[1], P[2]);
  f1[0] = q1*q1;
  tape1.Dependent(P, f1);
  tape1.optimize();

   // using my_erf, with CondExpGe function
  CppAD::Independent(P);
  AScalar q2 = dnormTrunc0_log2(P[0], P[1], P[2]);
  f2[0] = q2*q2;
  tape2.Dependent(P, f2);
  tape2.optimize(); 


  vecD val1(1);
  val1 = tape1.Forward(0, X);
  vecD grad1 = tape1.Jacobian(X);
  vecD hess_dense1 = tape1.Hessian(X, size_t(0));
  vecD hess_sparse1 = tape1.SparseHessian(X, w);
  
  cout << "Using CppAD/C++11 erf:" << val1[0] << endl;
  cout << "Gradient.\\n";

  for (size_t i=0; i<nvars; i++) {
    cout << "\\t" << grad1[i] << "\\n";
  }
  cout << "\\n\\n";
  
  cout << "Dense hessian.\\n";
  for (size_t i=0; i<nvars; i++) {
    for (size_t j=0; j<nvars; j++) {
      cout << "\\t" << hess_dense1[i*nvars+j];
    }
    cout << endl;
  }
  cout << endl << endl;

  cout << "Sparse hessian.\\n";
  for (size_t i=0; i<nvars; i++) {
    for (size_t j=0; j<nvars; j++) {
      cout << "\\t" << hess_sparse1[i*nvars+j];
    }
    cout << endl;
  }
  cout << endl << endl;

  vecD val2(1);
  val2 = tape2.Forward(0, X);
  vecD grad2 = tape2.Jacobian(X);
  vecD hess_dense2 = tape2.Hessian(X, size_t(0));
  vecD hess_sparse2 = tape2.SparseHessian(X, w);
  
  cout << "Using my_erf (with CondExpGe):" << val2[0] << endl;
  cout << "Gradient.\\n";
  for (size_t i=0; i<nvars; i++) {
    cout << "\\t" << grad2[i] << "\\n";
  }
  cout << "\\n\\n";

  cout << "Dense hessian.\\n";
  for (size_t i=0; i<nvars; i++) {
    for (size_t j=0; j<nvars; j++) {
      cout << "\\t" << hess_dense2[i*nvars+j];
    }
    cout << endl;
  }
  cout << endl << endl;


  cout << "Sparse hessian.\\n";
  for (size_t i=0; i<nvars; i++) {
    for (size_t j=0; j<nvars; j++) {
      cout << "\\t" << hess_sparse2[i*nvars+j];
    }
    cout << endl;
  } 
}
EOF
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
mv ../bug.$$ $name.cpp
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
