#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
Check special conditions that take effect when CPPAD_FOR_TMB is defined
during compliation.
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
int main(void)
{	bool ok = true;
	using std::cout;
	using CppAD::AD;
	//
	AD< AD<double> > x = 0.0;
	ok &= Value(x) == 0.0;
	//
	if( ok )
		return 0;
	return 1;
}
EOF
# -----------------------------------------------------------------------------
if [ ! -e ../cppad/configure.hpp ]
then
	echo
	echo 'Cannot find the file cppad/configure.hpp in directory ..'
	echo 'Must change into .. directory and run bin/run_cmake.sh'
	rm bug.$$
	exit 1
fi
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
mv ../bug.$$ $name.cpp
echo "g++ -I../.. -D CPPAD_FOR_TMB --std=c++11 -g $name.cpp -o $name"
g++ -I../.. -D CPPAD_FOR_TMB --std=c++11 -g $name.cpp -o $name
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
