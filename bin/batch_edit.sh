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
move_sed='s|list_files.sh|ls_files.sh|'
move_list='
'
cat << EOF > junk.sed
s|inline AD Abs(void) const;|inline AD abs_me(void) const;|
s|AD<Base> AD<Base>::Abs (void) const|AD<Base> AD<Base>::abs_me (void) const|
s|return x.Abs();|return x.abs_me();|
s|return abs( *x.ADBase() *);|return x.ADBase().abs_me();|
#
s|inline AD Sign(void) const;|inline AD sign_me(void) const;|
s|AD<Base> AD<Base>::Sign (void) const|AD<Base> AD<Base>::sign_me (void) const|
s|return x.Sign();|return x.sign_me();|
s|return sign( *x.ADBase() *);|return x.ADBase().sign_me();|
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
list_all=`bin/ls_files.sh`
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
