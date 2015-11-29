#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/batch_edit.sh" ]
then
	echo "bin/batch_edit.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# 1. With a few exceptions, move cppad/*.hpp to cppad/utility/*.hpp
# 2. Remove index commands that became redundant when the words in
#    section, head, and subhead commands were automatically indexed.
# 3. Change include gaurd from CPPAD_<NAME>_INCLUDED to CPPAD_<NAME>_HPP
# 4. Make the source code control $Id$ commands more uniform.
# 5. Remove invisible white space
# 6. Create utility.hpp (includes entire utilty directory)
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
#
git reset --hard
# ---------------------------------------------------------------------------
# cleanup from a previous run
#
list='
cppad/utility
cppad/utility.hpp
junk.sed
'
for name in $list
do
	if [ -e "$name" ]
	then
		echo_eval "rm -r $name"
	fi
done
echo_eval mkdir cppad/utility
# ---------------------------------------------------------------------------
# files that are hand edited
list='
add_copyright.sh
reduce_index.py
doxyfile.sh
batch_edit.sh
'
for file in $list
do
	echo_eval cp bin/new/$file bin/$file
done
chmod +x bin/batch_edit.sh
# ---------------------------------------------------------------------------
# cppad/*.h files (long ago deprecated)
list=`ls cppad/*.h | sed -e 's|cppad/||'`
for file in $list
do
	echo_eval git mv cppad/$file cppad/utility/$file
cat << EOF >> junk.sed
s|^\\tcppad/$file |\\tcppad/utility/$file |
EOF
done
# ---------------------------------------------------------------------------
# cppad/*.hpp files, move to utility drectory
# except for cppad.hpp, configure.hpp, configure.hpp.in
#
list=`ls cppad/*.hpp | sed  \
	-e 's|cppad/||' \
	-e '/cppad\.hpp/d' \
	-e '/configure\.hpp/d' \
	-e '/base_require.hpp/d' `
for file in $list
do
	echo_eval git mv cppad/$file cppad/utility/$file
cat << EOF >> junk.sed
s|^# include <cppad/$file>|# include <cppad/utility/$file>|
s|^\\tcppad/$file%|\\tcppad/utility/$file%|
s|^\\tcppad/$file |\\tcppad/utility/$file |
s|^\\tcppad/$file\$|\\tcppad/utility/$file|
s|^\\t%cppad/$file\$|\\t%cppad/utility/$file|
s|%cppad/$file%|%cppad/utility/$file%|
EOF
done
# ---------------------------------------------------------------------------
# junk.sed
cat << EOF >> junk.sed
# fix double word errors inside of mindex commands
# that are created by reduce_index.py below
s|mindex SparseJacobian jacobian|mindex SparseJacobian|
s|index OdeErrControl control|index OdeErrControl|
s|mindex OdeGearControl control|mindex OdeGearControl|
s|mindex bool CppAD::vector vectorBool|mindex bool vectorBool|
s|mindex Bender BenderQuad|mindex BenderQuad|
s|mindex log log1p|mindex log1p|
#
# change include gaurd to more standard fom
# (works better when you have name.h and name.hpp)
s|# *ifndef *CPPAD_\\([A-Z0-9_]*\\)_INCLUDED|# ifndef CPPAD_\\1_HPP|
s|# *define *CPPAD_\\([A-Z0-9_]*\\)_INCLUDED|# define CPPAD_\\1_HPP|
#
# change source code control Id commands to be more iniform
s|^# \$Id[^\$]*\$\$|# \$Id\$|
s|^// \$Id[^\$]*\$\$|// \$Id\$|
s|^/\\* *\$Id[^\$]*\$ *\\*/\$|// \$Id\$|
EOF
# ----------------------------------------------------------------------------
list=`git ls-files | sed -n \
	-e '/\.omh$/p'  \
	-e '/\.hpp$/p'  \
	-e '/\.hpp\.in$/p'  \
	-e '/\.cpp$/p'  \
	-e '/makefile.am$/p'  \
	-e '/\.h$/p'  \
	-e '/\.c$/p'  \
`
for file in $list
do
	# remove redundant index entries
	bin/reduce_index.py $file
	# see comments in junk.sed above
	sed -f junk.sed -i $file
	# remove invisible white space
	sed -e 's|[ \t][ \t]*$||' -e 's| *\t|\t|g' -i $file
done
# ----------------------------------------------------------------------------
# move changes corresponding to new include gaurd
sed -e 's|_INCLUDED|_HPP|' -i bin/check_define.sh
sed -e 's|_INCLUDED|_HPP|' -i bin/check_include_def.sh
# ----------------------------------------------------------------------------
# create utility.hpp and 
# change examples to use utility.hpp instead if individual include files
list=`ls cppad/utility/*.hpp`
for file in $list
do
	echo "# include <$file>" >> cppad/utility.hpp
	root=`echo $file | sed -e 's|.*/||'`
	sed \
	-e "s|$codei%# *include *<cppad/$root>|%# include <cppad/utility.hpp>|" \
	-e "s|$code *# *include *<cppad/$root>|%# include <cppad/utility.hpp>|" \
	-i $file
done
bin/add_copyright.sh cppad/utility.hpp
sed \
	-e '1s/$/\n# ifndef CPPAD_UTILITY_HPP\n# define CPPAD_UTILITY_HPP/' \
	-e '$s/$/\n# endif/' \
	-i cppad/utility.hpp
git add cppad/utility.hpp
#
sed -e 's|\tcppad/base_require.hpp|\tcppad/utility.hpp\n&|' -i makefile.am
# ----------------------------------------------------------------------------
# use utility.hpp in cppad.hpp
sed \
	-e '25,48d' \
	-e '24s|$|\n# include <cppad/utility.hpp>|' \
	-i cppad/cppad.hpp
# ----------------------------------------------------------------------------
# change examples and documentation to use utility.hpp
list=`git ls-files | sed -n \
	-e '/\.omh$/p'  \
	-e '/^example\//p' \
	-e '/^cppad_ipopt\/example\//p' \
`
for file in $list
do
	sed \
	-e 's|^# *include *<cppad/utility/[a-z_]*\.hpp>]*>|# include <cppad/utility.hpp>|' \
	-i $file
done
