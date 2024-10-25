#! /usr/bin/env bash
set -e -u
# ----------------------------------------------------------------------------
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2024 Bradley M. Bell
# ----------------------------------------------------------------------------
# This is a temporrary script used for the conversion to operator class objects
#
# file_in
# file where the original source code is located
file_in=zmul_op.hpp
#
# OpCode
# The OpCode for this operator
OpCode=ZmulvpOp
#
# n_res
# The number of results for this operator
n_res=1
# ----------------------------------------------------------------------------
#
# op_old
# lower case version of the OpCode for this operator without Op at the end
op_old=$( echo $OpCode | sed -e 's|Op$||' | tr [A-Z] [a-z] )
#
# op_lower, op_upper
# new version of the op code for this operator
op_lower=$(echo $op_old | sed -e 's|[vp][vp]$|_&|')
op_upper=$( echo $op_lower | tr [a-z] [A-Z] )
#
# file_out
file_out=${op_lower}.hpp
#
# grep, sed
source bin/grep_and_sed.sh
#
# dir
dir=include/cppad/local
#
# $dir/op_class
if [ ! -e $dir/op_class ]
then
   mkdir $dir/op_class
fi
#
# temp.sed
cat << EOF > temp.sed
/^template <class Base>/! b end
N
/_${op_old}/! b end
: loop
N
/\\n}\$/! b loop
s|^.*void \\([a-z]*\\)_${op_old}_op\\([^(]*\\)(|//\\n// \\1\\2\\nvoid \\1\\2(|
s|void [^)]*)|& const override|
s|^|   |
s|\\n|\\n   |g
s|   \$||
s|   \\n|\\n|g
p
#
: end
EOF
#
# $dir/op_class/$file_out
$sed -n -f temp.sed $dir/op/$file_in > $dir/op_class/$file_out
#
# temp.sed
cat << EOF > temp.sed
1,1s|^|# ifndef CPPAD_LOCAL_OP_CLASS_${op_upper}_HPP\\
# define CPPAD_LOCAL_OP_CLASS_${op_upper}_HPP\\
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later\\
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>\\
// SPDX-FileContributor: 2024 Bradley M. Bell\\
// ----------------------------------------------------------------------------\\
\\
# include <cppad/local/op_class/var_binary_op.hpp>\\
\\
namespace CppAD { namespace local { namespace op_class { // BEGIN namespace\\
template <class Base> class ${op_lower}_t : public var_binary_op_t<Base> \\
{\\
public: \\
   //\\
   // op2enum\\
   OpCode op2enum(void) const override\\
   {  return $OpCode; }\\
   //\\
   // n_res\\
   size_t n_res(void) const override\\
   {  return $n_res; }\\
   //\\
   // get_instance\\
   static ${op_lower}_t* get_instance(void) \\
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL; \\
      static ${op_lower}_t instance;\\
      CPPAD_ASSERT_NARG_NRES(\\
         instance.op2enum(), instance.n_arg(), instance.n_res()\\
      );\\
      return \\&instance;\\
   }\\
|
#
\$,\$s|\$|\\n};\\
}}} // END namespace\\
# endif|
EOF
#
# $dir/op_class/$file_out
$sed -f temp.sed -i $dir/op_class/$file_out
git add  $dir/op_class/$file_out
#
# $dir/op_class/enum2op.hpp
git checkout $dir/op_class/enum2op.hpp
$sed -i $dir/op_class/enum2op.hpp \
-e "s|^// BEGIN_SORT.*|&\n# include <cppad/local/op_class/${op_lower}.hpp>|" \
-e "s|^\(  *\)// BEGIN_SORT.*|&\n\1CPPAD_OP_CLASS_INSTANCE(${op_lower}, $OpCode)|"
bin/sort.sh $dir/op_class/enum2op.hpp
#
# temp.sed
cat << EOF > temp.sed
/BEGIN_SORT_[T]HIS_LINE_PLUS_/! b end
N
s|\\n# include.*|\\n# include <cppad/local/op_class/${op_lower}.hpp>&|
s|\\n\\( *\\)[a-z_]*<Base>::get_instance();|\\n\\1${op_lower}_t<Base>::get_instance();&|
#
: end
EOF
#
# $dir/op_class/enable_parallel.hpp
file=$dir/op_class/enable_parallel.hpp
git checkout $file
$sed -i $file -f temp.sed
bin/sort.sh $file

#
# temp.sed
cat << EOF > temp.sed
/case $OpCode:/! b one
: loop
N
/\\n *\\/\\/ -*\\n *\$/ ! b loop
d
b end
#
: one
/END_SORT_THIS_LINE/! b end
N
/op_class::enum2op<Base>/! b end
s|^ *|&case $OpCode:\\n&|
#
: end
EOF
#
# $dir/sweep/ forward*.hpp, reverse.hpp
list='
   forward0.hpp
   forward1.hpp
   forward2.hpp
   reverse.hpp
'
for name in $list
do
   file="$dir/sweep/$name"
   git checkout $file
   sed -i $file -f temp.sed
   bin/sort.sh $file
done
#
echo "Must add ${op_lower} to csv-table in $dir/op_class/var_base_op.hpp"
echo 'op_class.sh: OK'
exit 0
