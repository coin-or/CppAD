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
file_in=add_op.hpp
#
# OpCode
# The OpCode for this operator
OpCode=AddvvOp
# ----------------------------------------------------------------------------
#
# op_lower, op_upper
# lower case, upper case, name for this operator
op_lower=$( echo $OpCode | sed -e 's|Op$||' | tr [A-Z] [a-z] )
op_upper=$( echo $op_lower | tr [a-z] [A-Z] )
#
# file_out
file_out=${op_lower}_op.hpp
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
/_${op_lower}/! b end
: loop
N
/\\n}\$/! b loop
s|^.*void \\([a-z]*\\)_${op_lower}_op\\([^(]*\\)(|//\\n// \\1\\2\\nvoid \\1\\2(|
s|void [^)]*)|& const override|
s|^|   |
s|\\n|\\n   |g
s|   \$||
p
#
: end
EOF
#
# $dir/op_class/$op_lower
$sed -n -f temp.sed $dir/op/$file_in > $dir/op_class/$file_out
#
# temp.sed
cat << EOF > temp.sed
1,1s|^|# ifndef CPPAD_LOCAL_OP_CLASS_${op_upper}_OP_HPP\\
# define CPPAD_LOCAL_OP_CLASS_${op_upper}_OP_HPP\\
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later\\
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>\\
// SPDX-FileContributor: 2024 Bradley M. Bell\\
// ----------------------------------------------------------------------------\\
\\
# include <cppad/local/op_class/base_op.hpp>\\
\\
namespace CppAD { namespace local { // BEGIN namespace\\
template <class Base> class ${op_lower}_op_t : public base_op_t<Base> \\
{\\
public: \\
   //\\
   // get_instance\\
   static ${op_lower}_op_t* get_instance(void) \\
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL; \\
      static ${op_lower}_op_t instance;\\
      return \\&instance;\\
   }\\
|
#
\$,\$s|\$|\\n};\\
}} // END namespace\\
# endif|
#
s|  *\$||
s|  *\\n|\\n|g
EOF
#
# $dir/op_class/$op_lower
$sed -f temp.sed -i $dir/op_class//$file_out
#
#
echo 'temp.sh: OK'
exit 0
