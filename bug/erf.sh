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
Running erf.sh generates the message
	Error detected by false result for
    	0
	at line 1948 in the file 
    	../../cppad/local/optimize.hpp
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>

using CppAD::vector;
typedef CppAD::AD<double> AScalar;


typedef CppAD::vector<double> vecD;
typedef CppAD::vector<AScalar> vecA;

#ifndef M_LN_SQRT_2PI
#define M_LN_SQRT_2PI 0.9189385
#endif
#ifndef M_LN2
#define M_LN2 0.6931472
#endif

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

int main() {

  CppAD::ADFun<double> tape1;
  CppAD::ADFun<double> tape2;

  int nvars = 3;
  vecD X(nvars);
  X[0] = .1;
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

  // record tape for normal log density 
  CppAD::Independent(P);
  AScalar q1 = dnorm_log(P[0], P[1], P[2]);
  f1[0] = q1*q1;
  tape1.Dependent(P, f1);
  tape1.optimize();

   // record tape for truncated normal log density
  CppAD::Independent(P);
  AScalar q2 = dnormTrunc0_log(P[0], P[1], P[2]);
  f2[0] = q2*q2;
  tape2.Dependent(P, f2);

  // program crashes here
    tape2.optimize(); 


  vecD val1(1);
  val1 = tape1.Forward(0, X);
  vecD grad1 = tape1.Jacobian(X);

  vecD hess_dense1 = tape1.Hessian(X, size_t(0));
  vecD hess_sparse1 = tape1.SparseHessian(X, w);
     
  vecD val2(1);
  val2 = tape2.Forward(0, X);
  vecD grad2 = tape2.Jacobian(X);

  vecD hess_dense2 = tape2.Hessian(X, size_t(0));

  // crashes here if tape2 is not optimized
  vecD hess_sparse2 = tape2.SparseHessian(X, w);
  
  using std::cout;
  using std::endl;

  // Eigen::IOFormat fmt(3);

  cout << "Normal log density:" << val1[0] << endl;
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


  cout << "Trunctated normal log density:" << val2[0] << endl;
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
  cout << endl << endl;
  
 
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
