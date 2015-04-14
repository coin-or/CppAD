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
cat << EOF
This test results in a very long error message with boost-1.55.0 and 
gcc-4.9.2. Here is the gist of the message:
	/usr/include/boost/numeric/ublas/detail/matrix_assign.hpp:33:35:
	error: no match for ‘operator<’ 
	... snip ...
		return norm_inf (e1 - e2) < epsilon *
                                   ^
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>
#include <boost/numeric/ublas/lu.hpp>

int main() {
    typedef CppAD::AD<double> T;
    boost::numeric::ublas::matrix<T> a(5,5);
    boost::numeric::ublas::permutation_matrix<std::size_t> pert(5);
    // lu decomposition
    const std::size_t s = lu_factorize(a, pert);

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
echo "./$name"
if ! ./$name
then
	echo
	echo "$name.sh: Error"
	exit 1
fi
echo
echo "$name.sh: OK"
exit 0
