#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
name=`echo $0 | sed -e 's|^bug/||' -e 's|\.sh$||'`
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
cat << EOF
Description:
EOF
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>
int main(void)
{  bool ok = true;
   using std::cout;
   using CppAD::AD;
   using CppAD::vector;
   //
   cout << "1. copy bug/template.sh to bug/$name.sh\n";
   cout << "2. Edit bug/$name.sh replacing description and C++ source code\n";
   cout << "3. Run bug/$name.sh\n";
   cout << "Test passes (fails) if bug/$name.sh: OK (Error) echoed at end\n";
   //
   if( ok )
      return 0;
   return 1;
}
EOF
cxx_flags='-Wall -pedantic-errors -std=c++11 -Wshadow -Wconversion -g -O0'
eigen_dir="$HOME/prefix/eigen/include"
echo "g++ -I../../include -isystem $eigen_dir $cxx_flags $name.cpp -o $name"
g++ -I../../include -isystem $eigen_dir $cxx_flags $name.cpp -o $name
#
echo "build/bug/$name"
if ! ./$name
then
   echo
   echo "build/bug/$name: Error"
   exit 1
fi
echo
# -----------------------------------------------------------------------------
echo "bug/$name.sh: OK"
exit 0
