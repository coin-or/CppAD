#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Using g++ 4.8.1 results in the following error message:
#
# std_vector.cpp:8:7: error: no match for ‘operator|=’ (operand types are
# ‘std::vector<bool>::reference {aka std::_Bit_reference}’ and ‘bool’)
#  y[1] |= true;
#     ^
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
   mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
cat << EOF > $name.cpp
# include <vector>
int main(void)
{  int N = 1;
   std::vector<bool> y(N);
   for(int i = 0; i < N; i++ )
      y[i] = false;
   y[0]  = y[0] | true;
   y[1] |= true;
   return 0;
}
EOF
echo "g++ -g $name.cpp -o $name"
g++ -g $name.cpp -o $name
#
echo "./$name"
./$name
#
echo "rm $name $name.cpp"
rm $name $name.cpp
