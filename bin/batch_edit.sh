#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
revert_list='
'
move_list='
'
move_sed='s|mat_div|mat_inv|'
#
cat << EOF > junk.sed
s|vector<int>& *%x%|vector<%Base%>\\& %x%|
s|vector<int>& \\( *\\)%x%|vector<%Base%>\\&\\1%x%|
s|vector<int>& \\( *\\)x|vector<Base>\\&\\1x|
s|vector<int> \\( *\\)user_x;|vector<Base>\\1user_x;|
s|Integer( *parameter\\[arg\\[0\\]\\] )|parameter[arg[0]]|
s|Integer( %ax%[%i%];|Value( %ax%[%i%] );|
s|%x%\\[%i%\\] = std::numeric_limits<int>::max()|%x%[%i%] = CppAD::numeric_limits<%Base%>::quiet_NaN()|
s|std::numeric_limits<int>::max()|CppAD::numeric_limits<Base>::quiet_NaN()|
#
s|nr_left  *= size_t( x\\[0\\] );|nr_left  = size_t( CppAD::Integer( x[0] ) );|
s|n_middle *= size_t( x\\[1\\] );|n_middle = size_t( CppAD::Integer( x[1] ) );|
s|nc_right *= size_t( x\\[2\\] );|nc_right = size_t( CppAD::Integer( x[2] ) );|
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
list_all=`bin/ls_files.sh`
for file in $list_all
do
	if [ "$file" != 'bin/batch_edit.sh' ]
	then
		sed -f junk.sed  $file > junk.$$
		if ! diff $file junk.$$ > /dev/null
		then
			echo_eval sed -f junk.sed  -i $file
		fi
	fi
done
if [ -e junk.$$ ]
then
	rm junk.$$
fi
# ----------------------------------------------------------------------------
for old in $move_list
do
	new=`echo $old | sed -e "$move_sed"`
	if [ -e "$new" ]
	then
		rm -r "$new"
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
