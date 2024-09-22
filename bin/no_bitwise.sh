#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-24 Bradley M. Bell
# -----------------------------------------------------------------------------
if [ "$0" != "bin/no_bitwise.sh" ]
then
   echo "bin/no_bitwise.sh: must be executed from its parent directory"
   exit 1
fi
if [ "$#" != 0 ]
then
   echo 'bin/no_bitwise.sh: does not expect any arguments'
   exit 1
fi
# ----------------------------------------------------------------------------
# git reset --hard ?
response=''
while [ "$response" != 'yes' ] && [ "$response" != 'abort' ]
do
   read -p 'git reset --hard [yes/abort] ?' response
done
if [ "$response" == 'abort' ]
then
   echo 'bin/no_bitwise.sh: aborted'
   exit 1
fi
git reset --hard
#
# sed.$$
cat << EOF > sed.$$
s/] *& *\\([a-z][a-z_]*\\)\\[/] \\&\\& \\1[/g
s/] *| *\\([a-z][a-z_]*\\)\\[/] || \\1[/g
#
s/) *& *(/) \\&\\& (/g
s/) *| *(/) || (/g
#
s/] *& *\\([!(]\\)/] \\&\\& \\1/g
s/] *| *\\([!(]\\)/] || \\1/g
#
s/) *& *\\([A-Za-z][A-Za-z_]*[[(]\\)/) \\&\\& \\1/g
s/) *| *\\([A-Za-z][A-Za-z_]*[[(]\\)/) || \\1/g
#
s/] *\\& *$/] \\&\\&/
s/] | *$/] ||/
EOF
#
# file_list
file_list=$(git ls-files | sed -n \
   -e '/\.hpp$/p' \
   -e '/\.cpp/p'\
   -e '/example[/]abs_normal[/]qp_interior.hpp/d'
)
#
# *.hpp, *.cpp
# replace bitwise operands on logicals by logical operations.
for file in $file_list
do
   sed -i $file -f sed.$$
done
#
# sed.$$
rm sed.$$
echo 'no_bitwise.sh: OK'
exit 0
# The script above was run on 2024-09021 following by the following commands:
#
# git_new.sh to
# rm build/speed/cppad/*.out
# bin/speed_new.sh all
#
# Here is the results:
#
#     one=cur, two=new
# det_lu_rate_one = [ 1355694, 7815, 1052, 305.70, 132.33 ]
# det_lu_rate_two = [ 1326588, 7668, 1031, 305.01, 128.91 ]
# det_minor_rate_one = [ 780970, 449962, 154473, 33566, 5070 ]
# det_minor_rate_two = [ 775970, 444017, 146952, 32100, 4839 ]
# mat_mul_rate_one = [ 1633224, 5487, 750.39, 195.23, 75.93 ]
# mat_mul_rate_two = [ 1646152, 5404, 733.41, 199.21, 74.30 ]
# ode_rate_one = [ 275842, 1531, 430.16, 191.22, 113.16 ]
# ode_rate_two = [ 274601, 1508, 428.69, 192.79, 113.09 ]
# poly_rate_one = [ 1882647, 776135, 538845, 417875, 333869 ]
# poly_rate_two = [ 1840832, 748291, 539711, 414037, 340217 ]
# sparse_hessian_rate_one = [ 475.58, 41.42, 7.5392, 2.1304, 0.8436 ]
# sparse_hessian_rate_two = [ 478.50, 38.16, 7.6061, 2.1297, 0.9010 ]
# sparse_jacobian_rate_one = [ 1434, 332.37, 124.84, 35.39, 21.27 ]
# sparse_jacobian_rate_two = [ 1522, 338.94, 119.77, 34.85, 20.94 ]
