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
file_in=log_op.hpp
#
# OpCode
# The OpCode for this operator (whith out the Op at the end)
OpCode=LogOp
#
# n_res
# The number of results for this unary operator
n_res=1
# ----------------------------------------------------------------------------
#
# op_old
# lower case version of the OpCode for this operator without Op at the end
op_old=$( echo $OpCode | sed -e 's|Op$||' | tr [A-Z] [a-z] )
#
# op_lower, op_upper
# new version of the op code for this operator
op_lower=$(echo $op_old | sed -e 's|$|_v|')
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
# include <cppad/local/op_class/var_unary_op.hpp>\\
\\
namespace CppAD { namespace local { // BEGIN namespace\\
template <class Base> class ${op_lower}_t : public op_class::var_unary_op_t<Base> \\
{\\
public: \\
   //\\
   // get_instance\\
   static ${op_lower}_t* get_instance(void) \\
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL; \\
      static ${op_lower}_t instance;\\
      return \\&instance;\\
   }\\
   //\\
   // n_res\\
   size_t n_res(void) const override\\
   {  return $n_res; }\\
|
#
\$,\$s|\$|\\n};\\
}} // END namespace\\
# endif|
#
s|^\\( *\\)size_t *i_x *,|\\1const addr_t* arg,\\n\\1const Base* parameter,|
s|i_x|size_t(arg[0])|g
s|  *\$||
s|  *\\n|\\n|g
s|^\\( *size_t\\) *d *,|\\1        d         ,|
s|^\\( *size_t\\) *q *,|\\1        q         ,|
s|^\\( *size_t\\) *r *,|\\1        r         ,|
s|^\\( *size_t\\) *i_z *,|\\1        i_z       ,|
s|^\\( *const addr_t[*]\\) *arg *,|\\1 arg       ,|
s|\\(\\n *const Base[*]\\) *parameter *,|\\1   parameter ,|
s|^\\( *size_t\\) *cap_order *,|\\1        cap_order ,|
s|^\\( *Base[*]\\) *taylor *)|\\1         taylor    )|
s|^\\( *const Base[*]\\) *taylor *,|\\1   taylor    ,|
s|^\\( *size_t\\) *nc_partial *,|\\1        nc_partial,|
s|^\\( *Base[*]\\) *partial *)|\\1         partial   )|
s|2[.]0 [*]|Base(2.0) *|
EOF
#
# $dir/op_class/$file_out
$sed -f temp.sed -i $dir/op_class/$file_out
git add  $dir/op_class/$file_out
#
# $dir/op_class/enum2instance.hpp
git checkout $dir/op_class/enum2instance.hpp
sed -i $dir/op_class/enum2instance.hpp \
-e "s|^// BEGIN_SORT.*|&\n# include <cppad/local/op_class/${op_lower}.hpp>|" \
-e "s|^\(  *\)// BEGIN_SORT.*|&\n\1CPPAD_OP_CLASS_INSTANCE(${op_lower}, $OpCode)|"
bin/sort.sh $dir/op_class/enum2instance.hpp
#
cat << EOF > temp.sed
/case $OpCode:/! b one
: loop
N
/\\n *\\/\\/ -*\\n *\$/ ! b loop
d
#
: one
s|^\\(  *\\)// BEGIN_SORT.*|&\n\1case $OpCode:|
EOF
#
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
