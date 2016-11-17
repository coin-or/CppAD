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
cppad/local/base_complex.hpp
cppad/local/base_cond_exp.hpp
cppad/local/base_double.hpp
cppad/local/base_float.hpp
cppad/local/base_hash.hpp
cppad/local/base_limits.hpp
cppad/local/base_std_math.hpp
cppad/local/base_to_string.hpp
cppad/local/bender_quad.hpp
cppad/local/bool_fun.hpp
cppad/local/bool_valued.hpp
'
move_sed='s|/local/|/core/|'
#
cat << EOF > junk.sed
s|/local/base_complex.hpp|/core/base_complex.hpp|
s|/local/base_cond_exp.hpp|/core/base_cond_exp.hpp|
s|/local/base_double.hpp|/core/base_double.hpp|
s|/local/base_float.hpp|/core/base_float.hpp|
s|/local/base_hash.hpp|/core/base_hash.hpp|
s|/local/base_limits.hpp|/core/base_limits.hpp|
s|/local/base_std_math.hpp|/core/base_std_math.hpp|
s|/local/base_to_string.hpp|/core/base_to_string.hpp|
s|/local/bender_quad.hpp|/core/bender_quad.hpp|
s|/local/bool_fun.hpp|/core/bool_fun.hpp|
s|/local/bool_valued.hpp|/core/bool_valued.hpp|
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
