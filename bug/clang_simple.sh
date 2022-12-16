#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
name='clang_simple'
if [ "$0" != "bug/$name.sh" ]
then
   echo "usage: bug/$name.sh"
   exit 1
fi
# -----------------------------------------------------------------------------
if [ -e build/bug ]
then
   rm -r build/bug
fi
mkdir -p build/bug
cd build/bug
cmake ../..
# -----------------------------------------------------------------------------
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>
int main(void)
{  bool ok = true;
   using std::cout;
   using CppAD::AD;
   //
   std::cout << "Test for issue 31\n";
   CppAD::CheckSimpleVector<double, CppAD::vector<double> >();
   //
   if( ok )
      return 0;
   return 1;
}
EOF
cxx_flags='-g -O0'
eigen_dir="$HOME/prefix/eigen/include"
echo "clang++ -I../../include -isystem $eigen_dir $cxx_flags $name.cpp -o $name"
clang++ -I../../include -isystem $eigen_dir $cxx_flags $name.cpp -o $name
#
if ! ./$name
then
   echo
   echo "build/bug/$name: Error"
   exit 1
fi
echo
# ------------------------------------------------------------------------------
echo "./$name.sh: OK"
exit 0
