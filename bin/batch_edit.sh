#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Plan for batch edit of all files:
# 1. Do not update copyright date when do this edit
# 2. Remove an svn Id commands.
# 3. "included in the COPYING file" -> "included in the epl-v10.txt file"
# 4. Remove all omhelp index commands.
# 5. Change: $code $srcfile ...$$ $$ -> $srcfile ... $$
# 6. Convert tabs to spaces.
# 7. Change CPPAD_TESTVECTOR to a template type (see Eigen entry in wishlist).
# 8. Change : B^n to \B{R}^n and other simuilar uses of B as a space.
# 9. Change Vector<T> -> <T>Vector for T = Base, Size, Bool (T != Set)
# -----------------------------------------------------------------------------
spell_list='
'
revert_list='
'
move_list='
	cppad/core/reverse_subgraph.hpp
	example/sparse/reverse_subgraph.cpp
'
move_sed='s|reverse_subgraph|subgraph_reverse|'
#
cat << EOF > junk.sed
s|reverse_subgraph|subgraph_reverse|
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
# ---------------------------------------------------------------------------
list_all=`bin/ls_files.sh | sed \
	-e '/^compile$/d' \
	-e '/^config\.guess$/d' \
	-e '/^config\.sub$/d' \
	-e '/^configure$/d' \
	-e '/^depcomp$/d' \
	-e '/^epl-v10.txt$/d' \
	-e '/^gpl-v10.txt$/d' \
	-e '/^makefile.in$/d' \
	-e '/\/makefile.in$/d' \
	-e '/^missing$/d'`
edit_list=''
for file in $list_all
do
	if [ "$file" != 'bin/batch_edit.sh' ]
	then
		sed -f junk.sed  $file > junk.$$
		if ! diff $file junk.$$ > /dev/null
		then
			echo_eval sed -f junk.sed  -i $file
			edit_list="$edit_list $file"
		fi
	fi
done
if [ -e junk.$$ ]
then
	rm junk.$$
fi
# ----------------------------------------------------------------------------
for word in $spell_list
do
#
cat << EOF > junk.sed
/\$spell\$/! b skip
:loop
/\$\\\$/b check
N
b loop
#
: check
/$word/b skip
s/\$spell/&\\n\\t$word/
#
: skip
EOF
	for file in $edit_list
	do
		echo_eval sed -f junk.sed -i $file
	done
done
# ----------------------------------------------------------------------------
for old in $move_list
do
	new=`echo $old | sed -e "$move_sed"`
	if [ -e "$new" ]
	then
		rm -r "$new"
	fi
	dir=`echo $new | sed -e 's|/[^/]*$||'`
	if [ "$dir" != "$new" ] && [ ! -e "$dir" ]
	then
		echo_eval mkdir $dir
	fi
	echo_eval git mv $old $new
done
# ----------------------------------------------------------------------------
# files that were hand edited and cached using 'git_new.sh to'
if [ -e new ]
then
	echo_eval git_new.sh from
fi
# ----------------------------------------------------------------------------
for file in $revert_list
do
	echo_eval git checkout $file
done
# ----------------------------------------------------------------------------
cp $HOME/trash/batch_edit.sh bin/batch_edit.sh
echo "$0: OK"
exit 0
