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
Assert during optimization of an atomic function.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
# include <example/atomic/reciprocal.cpp>  // Get an atomic test function
using namespace CppAD;

int main(){
  vector<AD<double> > x(2);
  vector<AD<double> > y(1);
  x[0]=1; x[1]=1;
  AD<double> zero=0.0;  
  // Create the atomic reciprocal object
  atomic_reciprocal afun("atomic_reciprocal");
  vector< AD<double> > ax(1);
  vector< AD<double> > ay(1);
  Independent(x);
  ax[0] = x[0] + x[1];
  afun(ax,ay);
  y[0] = CondExpGt(ay[0],zero,zero,ay[0]);
  ADFun<double> F(x,y);
  
  F.optimize();  // <-- Line that cause problem
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
./$name
