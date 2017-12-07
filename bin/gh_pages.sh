#! /bin/bash -e
# $Id$
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
if [ "$0" != 'bin/gh_pages.sh' ]
then
	echo "bin/gh_pages.sh: must be executed from its parent directory"
	exit 1
fi
echo 'Under Construction: moving build.sh from gh-pages branch to master'
exit 1
temporary_dir="$HOME/trash"
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
# make sure .nojekyll exists
if [ ! -e .nojekyll ]
then
	echo 'gh-pages: .nojekyll should exist'
	exit 1
fi
# make sure that build.sh is the only file that has changed
list=`git status -s | sed -e '/ build.sh$/d' -e '/ .build.sh.swp$/d' `
if [ "$list" != '' ]
then
	git status -s | sed -e '/ build.sh$/d' -e '/ .build.sh.swp$/d'
	echo 'build.sh: git files other thant build.sh have changed'
	exit 1
fi
# -----------------------------------------------------------------------------
# copy build.sh to temporary directory
echo_eval cp build.sh $temporary_dir/cppad.build.sh
echo_eval git checkout build.sh
# -----------------------------------------------------------------------------
# checkout master
echo_eval git checkout master
if [ -d doc ]
then
	echo 'master: doc directory should not exist after checkout from gh-pages'
	exit 1
fi
#
version=`bin/version.sh get`
#
# change icon link to http://coin-or.github.io/CppAD/
sed bin/run_omhelp.sh \
	-e 's|https*://www\.coin-or\.org/CppAD/|../index.html|' > build.tmp
if diff build.tmp bin/run_omhelp.sh > /dev/null
then
	rm build.tmp
	echo "cannot change Home icon link in master:bin/run_omhelp.sh"
	exit 1
fi
mv build.tmp bin/run_omhelp.sh
chmod +x bin/run_omhelp.sh
#
# build documentation
echo_eval bin/run_omhelp.sh htm
#
# restore origina bin/run_omhelp.sh
git checkout bin/run_omhelp.sh
#
# move documentation to temporary directory
if [ -e $temporary_dir/cppad.doc ]
then
	echo_eval rm -r $temporary_dir/cppad.doc
fi
echo_eval mv doc $temporary_dir/cppad.doc
# -----------------------------------------------------------------------------
# checkout gh-pages
echo_eval git checkout gh-pages
#
# restore build.sh
echo_eval cp $temporary_dir/cppad.build.sh build.sh
echo_eval chmod +x build.sh
#
# determine which files to remove
list=`ls -a doc`
for file in $list
do
	if [ ! -e "$temporary_dir/cppad.doc/$file" ]
	then
		echo_eval git rm doc/$file
	fi
done
#
# copy new version of files
list=`ls $temporary_dir/cppad.doc`
for file in $list
do
	if [ ! -e doc/$file ]
	then
		echo "git add doc/$file"
	fi
	cp $temporary_dir/cppad.doc/$file doc/$file
done
# stage all the changes
git add doc/*
# -----------------------------------------------------------------------------
echo 'build.sh: OK'
echo 'following will commit changes:'
echo "git commit -m 'update gh-pages to cppad-$version'"
