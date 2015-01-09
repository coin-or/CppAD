#! /bin/bash -e
# $Id: template.sh 3370 2014-09-28 10:52:36Z bradbell $
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
#
# File created by Joao Leal
#
cat << EOF
Running this progam generates the following error in the file valgrind.log:

==19174== Conditional jump or move depends on uninitialised value(s)
==19174==    at 0x41681B: 
CppAD::vector<double> CppAD::ADFun<double>::Forward<CppAD::vector<double> >(
	unsigned long, unsigned long, CppAD::vector<double> const&
) (forward.hpp:376)
...
EOF
cat << EOF > bug.$$
#include <iostream>
#include <cppad/cppad.hpp>

int main(void) { // C++ source code
	using CppAD::vector;
	using CppAD::AD;
    
    vector<double> x(2);
    vector< AD<double> > ax(2);
    for (size_t i = 0; i < ax.size(); i++) {
        ax[i] = x[i] = 2.0;
    }
    Independent(ax);

    // assign some parameters
    AD<double> zero = double(0.);
    AD<double> one = double(1.);

    // dependent variable vector and indices
    vector< AD<double> > ay(2);

    // dependent variables
    ay[0] = ax[1];
    ay[1] = zero / ax[0];

    // create f : ax -> ay and vectors used for derivative calculations
    CppAD::ADFun<double> fun(ax, ay);
    
    vector<double> jac = fun.SparseJacobian(x);
    
    return 0;
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
echo "valgrind --error-exitcode=1 --track-origins=yes ./$name >& valgrind.log" 
if valgrind --error-exitcode=1 --track-origins=yes ./$name >& ../valgrind.log
then
	echo 'uninitialized.sh: OK'
	exit 0
else
	echo 'uninitialized.sh: Error (see valgrind.log)'
	exit 1
fi
