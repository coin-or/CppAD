#! /bin/bash -e
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
