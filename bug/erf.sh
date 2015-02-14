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
Sparse and Dense Hessians give different results.
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>
using CppAD::vector;
typedef CppAD::AD<double> AScalar;



typedef CppAD::vector<double> vec;
typedef CppAD::vector<AScalar> avec;


int main() {
  bool ok = true;
  double eps = 1.0 * std::numeric_limits<double>::epsilon();

  using std::cout;
  using std::endl;


  CppAD::ADFun<double> f;

  int nvars = 2;
  vec x(nvars);
  avec ax(nvars);
  ax[0] = x[0] = 0.5;
  ax[1] = x[1] = 0.0;

  vec w(1);
  w[0] = 1.0;

  // construct function
  avec ay(1);
  CppAD::Independent(ax);
  ay[0] = erf( 2.0 * ax[0] );
  f.Dependent(ax, ay);

  vec hess_dense = f.Hessian(x, 0);
  vec hess_sparse = f.SparseHessian(x, w);

  cout << "Dense hessian.\\n";
  cout << hess_dense << endl;

  cout << "Sparse hessian.\\n";
  cout << hess_sparse << endl;

  for(size_t i = 0; i < nvars * nvars; i++)
    ok &= CppAD::NearEqual( hess_dense[i], hess_sparse[i], eps, eps);

  // Define g(u) = erf(2 * u)
  // g'(u)   = 2 * erf'(2 * u)
  //         = 2 * exp( - 2 * u * 2 * u ) * 2 / sqrt(pi)
  //         = exp( - 4 * u * u ) * 4 / sqrt(pi)
  // g''(u)  = - exp( - 4 * u * u ) * 32 * u / sqrt(pi) 
  double root_pi      = std::sqrt( 4.0 * atan(1.0) );
  double check_hess_0 = -std::exp(-4.0 * x[0] * x[0]) * 32.0 * x[0] / root_pi;
  cout << "check hess[0] = " << check_hess_0 << endl;

  ok &= CppAD::NearEqual(hess_dense[0], check_hess_0, eps, eps);

  if( ok )
    return 0;
  return 1;
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
