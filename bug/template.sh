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
Description
EOF
cat << EOF > bug.$$
# include <cppad/cppad.hpp>
int main(void)
{	bool ok = true;
	using std::cout;
	//
	cout << "1. copy template.sh to <name>.sh\n";
	cout << "2. Edit <name>.sh replacing description and C++ source code\n";
	cout << "3. Run ./<name>.sh\n";
	cout << "where <name> is a name that describes the bug\n";
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
