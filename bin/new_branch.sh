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
if [ "$0" != "bin/new_branch.sh" ]
then
	echo 'bin/new_branch.sh: must be executed from its parent directory'
	exit 1
fi
if [ "$1" == '' ] || [ "$2" == 'trunk' ]
then
	echo 'usage:  bin/new_branch.sh svn_branch_path'
	echo 'copies the current trunk to svn_branch_path'
	exit 1
fi
svn_branch_path="$1"
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
# some settings
svn_repository='https://projects.coin-or.org/svn/CppAD'
work_directory='build/work'
# -----------------------------------------------------------------------------
# check that remote master is in sync
hash_origin=`git show-ref origin/master | sed -e 's|^\([0-9a-f]*\).*|\1|'`
hash_list=`git show-ref master | sed -e 's|^\([0-9a-f]*\).*|\1|'`
for hash in $hash_list
do
	if [ "$hash" != "$hash_origin" ]
	then
		echo_eval git show-ref master
		echo 'exiting because hash codes are different'
		exit 1
	fi
done
# -----------------------------------------------------------------------------
# make sure work directory exists
if [ ! -d "$work_directory" ]
then
	echo_eval mkdir -p "$work_directory"
fi
# -----------------------------------------------------------------------------
# checkout svn version of directory
svn_directory="$work_directory/svn"
if [ -e "$svn_directory" ]
then
	echo 'Use existing svn direcory'
	echo "	$svn_directory"
	read -p '? [y/n] ' response
	if [ "$response" != 'y' ]
	then
		echo_eval rm -r $svn_directory
	else
		echo_eval svn revert --recursive $svn_directory
		echo_eval svn update $svn_directory
		svn_status=`svn status $svn_directory | \
			sed -n -e '/^\?/p' | sed -e 's|^\? *||'`
		for file_name in $svn_status
		do
			echo_eval rm $file_name
		done
	fi
fi
if [ ! -e "$svn_directory" ]
then
	echo_eval svn checkout $svn_repository/trunk $svn_directory
fi
# ----------------------------------------------------------------------------
# git hash code corresponding to current version of trunk
svn_revision=`svn info $svn_directory | \
	sed -n -e '/^Last Changed Rev:/p' | sed -e 's|^Last Changed Rev: *||'`
hash_svn=`svn log -r $svn_revision $svn_directory | \
	sed -n -e '/^end *hash *code:/p' | sed -e 's|^end *hash *code: *||'`
if [ "$hash_origin" != "$hash_svn" ]
then
	echo "$hash_origin hash code for master"
	echo "$hash_svn hash code for trunk"
	echo 'Exiting because hash codes are different.'
	echo 'Use the following command to fix this'
	echo '	bin/push_git2svn.py trunk'
	exit 1
fi
# -----------------------------------------------------------------------------
echo_eval git checkout master
git_branch_name=`echo $svn_branch_path | sed -e 's|^branches/||'`
echo_eval git branch $git_branch_name
echo_eval git checkout $git_branch_name
git push --set-upstream origin $git_branch_name
# -----------------------------------------------------------------------------
cat << EOF > $svn_directory/new_branch.log
copy trunk to:   $svn_branch_path
start hash code: $hash_origin
end   hash code: $hash_origin
EOF
echo_eval svn copy --file new_branch.log \
	$svn_repository/trunk $svn_repository/$svn_branch_path
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
