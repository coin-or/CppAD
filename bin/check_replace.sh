#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/check_replace.sh" ]
then
	echo "bin/check_replace.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
pattern='<!-- define'
extensions='.cpp .hpp'
directories='cppad cppad/local'
list=`find_files.sh "$pattern" "$extensions" "$directories"`
echo '------------------------------------------------------------------------'
echo 'check replacement text in following files:'
for file in $list
do
	echo "$file"
	bin/replace_html.py $file $file.rep
	if ! diff $file $file.rep > /dev/null
	then
		cat << EOF
check_replace.sh: Error: The replacement text in $file
does not match its replacement commands; see
	diff $file $file.rep
EOF
		exit 1
	fi 
	rm $file.rep
done
echo '------------------------------------------------------------------------'
exit 0
