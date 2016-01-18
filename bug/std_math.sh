#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
There was a ambiguity in the standard math functions when using both
CppAD and std namespaces. This has been fixed. Note that the ambugility
between std::vector and CppAD::vector remains (because they are different).
EOF
cat << EOF > bug.$$
#include <cmath>
#include <cppad/cppad.hpp>

int main() {
	using namespace std;
	using namespace CppAD;

	double a = 1.0, b, c;
	CppAD::AD<double> ax = 1.0, bx, cx;
	std::vector<double> v(3);
	std::vector<CppAD::AD<double> > vx(3);

	b  = fabs(a);
	bx = fabs(ax);
	c  = pow(b,3);
	cx = pow(bx,3);
	v[0] = a;
	vx[0] = ax;

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
