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
cppad/local/capacity_order.hpp
cppad/local/check_for_nan.hpp
cppad/local/checkpoint.hpp
cppad/local/compare.hpp
cppad/local/compute_assign.hpp
cppad/local/cond_exp.hpp
cppad/local/convert.hpp
cppad/local/cppad_assert.hpp
cppad/local/define.hpp
cppad/local/dependent.hpp
cppad/local/discrete.hpp
cppad/local/div_eq.hpp
cppad/local/div.hpp
cppad/local/drivers.hpp
cppad/local/epsilon.hpp
cppad/local/erf.hpp
cppad/local/expm1.hpp
'
move_sed='s|/local/|/core/|'
#
cat << EOF > junk.sed
s|/local/capacity_order.hpp|/core/capacity_order.hpp|
s|/local/check_for_nan.hpp|/core/check_for_nan.hpp|
s|/local/checkpoint.hpp|/core/checkpoint.hpp|
s|/local/compare.hpp|/core/compare.hpp|
s|/local/compute_assign.hpp|/core/compute_assign.hpp|
s|/local/cond_exp.hpp|/core/cond_exp.hpp|
s|/local/convert.hpp|/core/convert.hpp|
s|/local/cppad_assert.hpp|/core/cppad_assert.hpp|
s|/local/define.hpp|/core/define.hpp|
s|/local/dependent.hpp|/core/dependent.hpp|
s|/local/discrete.hpp|/core/discrete.hpp|
s|/local/div_eq.hpp|/core/div_eq.hpp|
s|/local/div.hpp|/core/div.hpp|
s|/local/drivers.hpp|/core/drivers.hpp|
s|/local/epsilon.hpp|/core/epsilon.hpp|
s|/local/erf.hpp|/core/erf.hpp|
s|/local/expm1.hpp|/core/expm1.hpp|
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
