#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
if [ "$0" != "bug/$name.sh" ]
then
	echo "usage: bug/$name.sh"
	exit 1
fi
cat << EOF
Description
EOF
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>
int main(void)
{	bool ok = true;
	using std::cout;
	using CppAD::AD;
	//
	cout << "1. copy bug/template.sh to bug/<name>.sh\n";
	cout << "2. Edit bug/<name>.sh replacing description and C++ source code\n";
	cout << "3. Run bug/<name>.sh\n";
	cout << "where <name> is a name that describes the bug\n";
	//
	if( ok )
		return 0;
	return 1;
}
EOF
# -----------------------------------------------------------------------------
if [ ! -e cppad/configure.hpp ]
then
	echo
	echo 'Cannot find the file cppad/configure.hpp.'
	echo 'Must run bin/run_cmake.sh to create it.'
	rm $name.cpp
	exit 1
fi
if [ -e build/bug ]
then
	rm -r build/bug
fi
mkdir -p build/bug
mv $name.cpp build/bug/$name.cpp
cd build/bug
cxx_flags='-Wall -pedantic-errors -std=c++11 -Wshadow -Wconversion -g -O0'
eigen_dir="$HOME/prefix/eigen/include"
echo "g++ -I../.. -isystem $eigen_dir $cxx_flags $name.cpp -o $name"
g++ -I../.. -isystem $eigen_dir $cxx_flags $name.cpp -o $name
#
echo "build/bug/$name"
if ! ./$name
then
	echo
	echo "build/bug/$name: Error"
	exit 1
fi
echo
echo "./$name.sh: OK"
exit 0
