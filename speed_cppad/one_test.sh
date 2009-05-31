# $Id$
# ---------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# ---------------------------------------------------------------------------
#
# Run one of the tests
if [ "$1" = "" ]
then
	echo "one_test.sh testname files"
	echo "where testname is the name of the test function run by Speed"
	echo "and files are the *.cpp file including it's extension."
	exit
fi
if [ -e test_one.exe ]
then
	rm test_one.exe
fi
sed < speed.cpp > test_one.cpp \
-e '/SpeedTest(/d' \
-e "s/.*This line used by one_test.sh.*/	SpeedTest($1, 5, -1, 1);/"  
#
echo "g++ test_one.cpp $2 -o test_one.exe  \\"
echo "	-DNDEBUG -Wall -ansi -pedantic-errors -std=c++98 \\"
echo "	-I.. -I/usr/include/boost-1_33"
g++ test_one.cpp $2  -o test_one.exe  \
	-DNDEBUG -Wall -ansi -pedantic-errors -std=c++98 \
	-I.. -I/usr/include/boost-1_33 
#
echo "./test_one.exe"
./test_one.exe
