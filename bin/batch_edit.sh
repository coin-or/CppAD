#! /bin/bash -e
# $Id:$
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
#
git reset --hard
cp bin/new/reduce_index.py bin/reduce_index.py
cp bin/new/doxyfile.sh bin/doxyfile.sh
cp bin/new/batch_edit.sh bin/batch_edit.sh
#
if [ ! -e cppad/utility ]
then
	mkdir cppad/utility
fi
if [ -e junk.sed ]
then
	rm junk.sed
fi
list=`ls cppad/*.h | sed -e 's|cppad/||'`
for file in $list
do
	echo_eval git mv cppad/$file cppad/utility/$file
cat << EOF >> junk.sed
s|^\\tcppad/$file |\\tcppad/utility/$file |
EOF
done
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
cat << EOF >> junk.sed
s|mindex SparseJacobian jacobian|mindex SparseJacobian|
s|index OdeErrControl control|index OdeErrControl|
s|mindex OdeGearControl control|mindex OdeGearControl|
s|mindex bool CppAD::vector vectorBool|mindex bool vectorBool|
s|mindex Bender BenderQuad|mindex BenderQuad|
s|mindex log log1p|mindex log1p|
#
s|# *ifndef *CPPAD_\\([A-Z0-9_]*\\)_INCLUDED|# ifndef CPPAD_\\1_HPP|
s|# *define *CPPAD_\\([A-Z0-9_]*\\)_INCLUDED|# define CPPAD_\\1_HPP|
#
/^# \\$Id\\$\\\$/d
/^# \\$Id:\\$\\\$/d
/^\\/\\/ \\$Id\\$\\\$/d
/^\\/\\* \\$Id\\$ \\*\\/\\\$/d
EOF
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
	bin/reduce_index.py $file
	sed -f junk.sed -i $file
	sed -e 's|[ \t][ \t]*$||' -e 's| *\t|\t|g' -i $file
done
#
sed -e 's|_INCLUDED|_HPP|' -i bin/check_define.sh
sed -e 's|_INCLUDED|_HPP|' -i bin/check_include_def.sh
