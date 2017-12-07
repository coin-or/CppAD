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
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
# make sure we are on the master branch
branch=`git branch | sed -n -e '/^\*/p'`
if [ "$branch" != '* master' ]
then
	echo 'bin/gh_pages can only be executed from master branch'
	exit 1
fi
# make sure that bin/gh_pages is the only file that has changed
list=`git status -s | sed -e '/^ M bin\/gh_pages.sh$/d'`
if [ "$list" != '' ]
then
	git status -s
	echo 'bin/gh_pages.sh: git files other thant bin/gh_pages.sh have changed.'
	exit 1
fi
#
# copy gh_pages.sh to a build directory for safe keeping
if [ ! -d build/tmp ]
then
	mkdir -p build/tmp
fi
echo_eval cp bin/gh_pages.sh build/tmp/gh_pages.sh
#
# revert copy of bin/gh_pages.sh so that status for master is clean
echo_eval git checkout bin/gh_pages.sh
#
if diff bin/gh_pages.sh build/tmp/gh_pages.sh > /dev/null
then
	gh_pages_changed='no'
else
	gh_pages_changed='yes'
fi
# -----------------------------------------------------------------------------
# version of documentation we are building
version=`bin/version.sh get`
#
# build documentation
bin/run_omhelp.sh htm clean gh_pages
#
# move doc to build/tmp/doc directory for safe keeping
if [ -e 'build/tmp/doc' ]
then
	echo_eval rm -r build/tmp/doc
fi
echo_eval mv doc build/tmp/doc
# -----------------------------------------------------------------------------
# checkout gh-pages
echo_eval git checkout gh-pages
#
# make sure .nojekyll exists
if [ ! -e .nojekyll ]
then
	echo 'bin/gh-pages: .nojekyll should exist in gh-pages branch'
	exit 1
fi
#
# determine which files to remove
list=`ls -a doc`
for file in $list
do
	if [ ! -e "build/tmp/doc/$file" ]
	then
		echo_eval git rm doc/$file
	fi
done
#
# copy new version of files
list=`ls build/tmp/doc`
for file in $list
do
	if [ ! -e doc/$file ]
	then
		echo "git add doc/$file"
	fi
	cp build/tmp/$file doc/$file
done
#
# stage all the changes
git add doc/*
# -----------------------------------------------------------------------------
echo 'The following will commit changes to gh-pages branch:'
echo "	git commit -m 'update gh-pages to cppad-$version'"
if [ "$gh_pages_changed" == 'yes' ]
then
	echo 'The following will restore the bin/gh_pages.sh file'
	echo '	git checkout master'
	echo '	cp build/tmp/gh_pages.sh bin/gh_pages.sh'
fi
echo 'bin/gh_pages.sh: OK'
exit 0
