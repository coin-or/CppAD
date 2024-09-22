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
