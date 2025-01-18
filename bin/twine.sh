#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-25 Bradley M. Bell
# -----------------------------------------------------------------------------
# bin/twine.sh branch_or_tag
# 1. This program muse be executed from the main or master branch
# 2. branch_or_tag can be a branch or a tag. If it is a branch, it must be
#    main or master.
# 3. If branch_or_tag is a tag, the corresponding version is upladed to pypi.
#    Otherwise it is uploaded to testpipi.
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != "bin/twine.sh" ]
then
   echo "bin/twine.sh: must be executed from its parent directory"
   exit 1
fi
if [ ! -e .git ]
then
   echo 'bin/twine.sh: Cannot find .git in curretn working directory'
   exit 1
fi
#
# branch_or_tag
if [ $# != 1 ]
then
   echo 'usage: bin/twine.sh: branch_or_tag'
   echo 'where branch_or_tag is master, main, or a git tag'
   exit 1
fi
branch_or_tag="$1"
#
# branch
branch=$(git branch --show-current)
if [ "$branch" != 'master' ] && [ "$branch" != 'main' ]
then
   echo 'bin/twine.sh: must execute on the main or master branch'
   exit 1
fi
#
# repository
if [ "$branch_or_tag" == "$branch" ]
then
   repository='testpypi'
else
   repository='pypi'
fi
#
# TOKEN
if [ -z "${TOKEN+x}" ]
then
   echo echo "bin/twine.sh: must export TOKEN=<API token for $repository>"
   echo 'see https://pypi.org/help/#apitoken'
   exit 1
fi
#
# dist
if [ -e dist ]
then
   rm -r dist
fi
if [ "$branch_or_tag" == "$branch" ]
then
   echo_eval python -m build
else
   if ! git show-ref $branch_or_tag | grep '/tags/' > /dev/null
   then
      echo "bin/twine.sh: $branch_or_tag is not the current branch or a tag"
      exit 1
   fi
   git checkout --quiet $branch_or_tag
   echo_eval python -m build
   git checkout $branch
fi
#
# twind
echo_eval twine upload --repository $repository  dist/* -u __token__ -p $TOKEN
#
echo 'twine.sh: OK'
exit 0
