#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
echo "create reachable.cpp"
cat << EOF > reachable.cpp
# include <iostream>
# include <vector>
# include <cppad/cppad.hpp>

# define SHOW_ERROR 1

int main (int argc, char *argv[])
{
  CppAD::vector< CppAD::AD<double> > x(2);
  x.resize(0);

  // vagrind will generate an error message about a reachable block of memory.
  // The intention is that, when no multi-threading is done, the user should
  // note have to worry about the CppAD::thread_alloc memory allocator; i.e.,
  // this should not be necessary.
# if ! SHOW_ERROR
  CppAD::thread_alloc::free_available(0);
# endif

  return 0;
}
EOF
#
echo "g++ -g -I..  reachable.cpp -o reachable"
g++ -g -I.. reachable.cpp -o reachable
#
echo "valgrind --leak-check=full --show-reachable=yes ./reachable"
valgrind --leak-check=full --show-reachable=yes ./reachable
#
echo "rm reachable reachable.cpp"
rm reachable reachable.cpp
