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


  vec val1(1);
  val1 = f.Forward(0, x);
  vec hess_dense1 = f.Hessian(x, size_t(0));
  vec hess_sparse1 = f.SparseHessian(x, w);

  cout << "Dense hessian.\\n";
  cout << hess_dense1 << endl;

  cout << "Sparse hessian.\\n";
  cout << hess_sparse1 << endl;

  for(size_t i = 0; i < nvars * nvars; i++)
    ok &= CppAD::NearEqual( hess_dense1[i], hess_sparse1[i], eps, eps);

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
