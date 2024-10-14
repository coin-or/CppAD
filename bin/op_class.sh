#! /usr/bin/env bash
set -e -u
# ----------------------------------------------------------------------------
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2024 Bradley M. Bell
# ----------------------------------------------------------------------------
# This is a temporrary script used for the conversion to operator class objects
#
# file
# file where the original source code is located
file=add_op.hpp
#
# op_lower
# lower case, mixed case, uppser case version of this operator.
op_lower=addvv
op_mixed=Addvv
op_upper=ADDVV
# ----------------------------------------------------------------------------
git reset --hard
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
$sed -n -f temp.sed $dir/op/$file > $dir/op_class//${op_lower}_op.hpp
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
\$,\$s|\$|\\n};\\
}} // END namespace\\
# endif|
EOF
#
# $dir/op_class/$op_lower
$sed -f temp.sed -i $dir/op_class//${op_lower}_op.hpp
#
#
echo 'temp.sh: OK'
exit 0
