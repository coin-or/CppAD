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
if [ "$0" != "bin/mv_branch.sh" ]
then
	echo 'bin/mv_branch.sh: must be executed from its parent directory'
	exit 1
fi
if [ ! -e '.git/svn' ]
then
	echo 'bin/mv_branch.sh: must be executed in the git_svn repository'
	exit 1
fi
if [ "$2" == '' ]
then
	echo 'usage:  bin/mv_branch.sh old_branch_name new_branch_name'
	exit 1
fi
old_branch_name="$1"
new_branch_name="$2"
repository='https://projects.coin-or.org/svn/CppAD'
old_branch_path="branches/$old_branch_name"
new_branch_path="branches/$new_branch_name"
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
pause() {
	response=''
	while [ "$response" != 'y' ] && [ "$response" != 'n' ]
	do
		read -p 'Continue [y/n] ' response
	done
	if [ "$response" != 'y' ]
	then
		exit 1
	fi
}
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
if [ "$old_branch_name" == "$new_branch_name" ]
then
	echo "mv_branch.sh: old and new branch names are the same"
	exit 1
fi
for branch_path in $old_branch_path $new_branch_path
do
	for name in 'master' 'trunk'
	do
		if [ "$branch_path" == "$name" ]
		then
			echo "mv_branch.sh: neither old or new branch path can be $name"
			exit 1
		fi
	done
done
old_fetch="$old_branch_path:refs/remotes/svn/$old_branch_path"
if ! grep "fetch *= *$old_fetch" .git/config > /dev/null
then
	echo "mv_branch.sh: cannot find $old_branch_path in ./git/config"
	exit 1
fi
svn_remote_dir=".git/svn/refs/remotes/svn"
dir=`echo $new_branch_path | sed -e 's|/[^/]*$||'`
if [ ! -e "$svn_remote_dir/$dir" ]
then
	echo 'mv_branch.sh: Must first create the directory'
	echo "	$svn_remote_dir/$dir"
	exit 1
fi
dir=`echo $new_branch_name | sed -e 's|/.*||'`
if [ "$dir" != "$new_branch_name" ]
then
	if ! svn list $repository/branches | grep "$dir/" > /dev/null
	then
		echo 'mv_branch.sh: Must frist create the directory'
		echo "	$repository/branches/$dir"
	fi
fi
# -----------------------------------------------------------------------------
# make sure repsitories are in sync
echo_eval git fetch github
echo "git show-ref --hash $old_branch_name | head -1"
git show-ref --hash $old_branch_name | head -1
echo_eval git show-ref --hash github/$old_branch_name
pause
# -----------------------------------------------------------------------------
# svn
msg="Moving $old_branch_path to $new_branch_path"
cmd="svn mv $repository/$old_branch_path $repository/$new_branch_path"
echo "$cmd -m \"$msg\""
$cmd -m "$msg"
pause
# ----------------------------------------------------------------------------
# change the name locally
echo_eval git branch -m $old_branch_name $new_branch_name
pause
# ----------------------------------------------------------------------------
# change the remote github name for the branch
echo_eval git push github --set-upstream $new_branch_name
echo_eval git push github :$old_branch_name
pause
# ----------------------------------------------------------------------------
# change the remote svn name for the branch
#
# backup current version of .git/config
index='1'
while [ -e .git/config.$index ]
do
	index=`expr $index + 1`
done
echo_eval cp .git/config .git/config.$index
#
# change the svn fetch command
new_fetch="$new_branch_path:refs/remotes/svn/$new_branch_path"
#
sed -e "s|$old_fetch|$new_fetch|" -i .git/config
echo_eval diff .git/config.$index .git/config
pause
#
# move the svn fetch infromation
svn_remote_dir=".git/svn/refs/remotes/svn"
echo_eval mv $dir/$old_branch_path $dir/$new_branch_path
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
