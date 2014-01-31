#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#! /bin/bash -e
echo 'This script does not work properly on el6; see'
echo 'https://bugzilla.redhat.com/show_bug.cgi?id=896116'
echo
#
if [ -e build ]
then
	rm -r build
fi
mkdir build
cd    build
cat << EOF > hello_one.cpp
# include <iostream>
int main(void)
{	std::cout << "hello_one" << std::endl << std::endl;
	return 0;
}
EOF
cat << EOF > hello_two.cpp
# include <iostream>
int main(void)
{	std::cout << "hello_two" << std::endl << std::endl;
	return 0;
}
EOF
cat << EOF > CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#
PROJECT(hello)
#
ADD_EXECUTABLE(hello_one EXCLUDE_FROM_ALL hello_one.cpp )
ADD_EXECUTABLE(hello_two EXCLUDE_FROM_ALL hello_two.cpp )
#
ADD_CUSTOM_TARGET(check_one hello_one DEPENDS hello_one)
ADD_CUSTOM_TARGET(check_two hello_two DEPENDS hello_two)
#
ADD_CUSTOM_TARGET(check DEPENDS check_one check_two)
EOF
#
uname -a
cmake --version
cmake .
make check
