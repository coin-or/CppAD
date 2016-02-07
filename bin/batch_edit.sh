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
move_sed='s|list_files.sh|ls_files.sh|'
move_list='
'
cat << EOF > junk.sed
s|^# cppad_has_colpack, colpack_libs|&, cppad_lib|
s|^\\tSET( colpack_libs "ColPack" )|&\\
	SET( cppad_lib "cppad_lib" )|
s|^\\tSET( colpack_libs "" )|&\\
	SET( cppad_lib "" )|
s|^\\tcppad_lib\$|\\t\${cppad_lib}|
#
s|^ADD_SUBDIRECTORY(cppad_lib)|IF( colpack_prefix )\\
	&\\
ENDIF( colpack_prefix)|
EOF
# -----------------------------------------------------------------------------
if [ $0 != "bin/batch_edit.sh" ]
then
	echo "bin/batch_edit.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
cp bin/batch_edit.sh $HOME/trash/batch_edit.sh
git reset --hard
cp $HOME/trash/batch_edit.sh bin/batch_edit.sh
# ---------------------------------------------------------------------------
list_all=`git ls-files`
for file in $list_all
do
	if [ "$file" != 'bin/batch_edit.sh' ]
	then
		echo_eval sed -f junk.sed -i $file
	fi
done
# ----------------------------------------------------------------------------
for old in $move_list
do
	new=`echo $old | sed -e "$move_sed"`
	echo_eval git mv $old $new
done
# ----------------------------------------------------------------------------
# files that were hand edited and cached using 'git_new.sh to'
if [ -e new ]
then
	echo_eval git_new.sh from
fi
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
