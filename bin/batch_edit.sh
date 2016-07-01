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
/if( *user_[ti]*x.size() *!= *user_n *)/! b one
N
s|^\\([\\t]*\\).*\\(user_[ti]*x.resize( *user_n *);\\)\$|\1\2|
: one
/if( *user_[ti]y.size() *!= *user_m *)/! b two
N
s|^\\([\\t]*\\).*\\(user_[ti]y.resize( *user_m *);\\)\$|\1\2|
: two
# ------------------------------------------------------------------------
/if( *user_tx.size() *!= *user_n *\\* *user_q1 *)/! b three
N
s|^\\([\\t]*\\).*\\(user_tx.resize( *user_n *\\* *user_q1 *);\\)\$|\1\2|
: three
/if( *user_ty.size() *!= *user_m *\\* *user_q1 *)/! b four
N
s|^\\([\\t]*\\).*\\(user_ty.resize( *user_m *\\* *user_q1 *);\\)\$|\1\2|
: four
# ------------------------------------------------------------------------
/if( *user_tx_one.size() *!= *user_n *\\* *user_q1 *)/! b five
N
s|^\\([\\t]*\\).*\\(user_tx_one.resize( *user_n *\\* *user_q1 *);\\)\$|\1\2|
: five
/if( *user_ty_one.size() *!= *user_m *\\* *user_q1 *)/! b six
N
s|^\\([\\t]*\\).*\\(user_ty_one.resize( *user_m *\\* *user_q1 *);\\)\$|\1\2|
: six
# ------------------------------------------------------------------------
/if( *user_tx_all.size() *!= *user_n *\\* *(q \\* r + 1) *)/! b seven
N
s|^\\([\\t]*\\).*\\(user_tx_all.resize( *user_n *\\* *(q \\* r + 1) *);\\)\$|\1\2|
: seven
/if( *user_ty_all.size() *!= *user_m *\\* *(q \\* r + 1) *)/! b eight
N
s|^\\([\\t]*\\).*\\(user_ty_all.resize( *user_m *\\* *(q \\* r + 1) *);\\)\$|\1\2|
: eight
# ------------------------------------------------------------------------
/if( *pack_[rs].size() *!= *user_[nm] *\\* *user_q *)/! b nine
N
s|^\\([\\t{]*\\).*\\(pack_[rs].resize( *user_[nm] *\\* *user_q *);\\)\$|\1\2|
: nine
# ------------------------------------------------------------------------
/if( *bool_[rs].size() *!= *user_[nm] *\\* *user_q *)/! b eleven
N
s|^\\([\\t{]*\\).*\\(bool_[rs].resize( *user_[nm] *\\* *user_q *);\\)\$|\1\2|
: eleven
# ------------------------------------------------------------------------
/if( *set_[rs].size() *!= *user_[nm] *)/! b thirteen
N
s|^\\([\\t{]*\\).*\\(set_[rs].resize( *user_[nm] *);\\)\$|\1\2|
: thirteen
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
