#! /bin/bash -e
# $Id:$
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
if [ $0 != "bin/tag_month.sh" ]
then
	echo "bin/tag_month.sh: must be executed from its parent directory"
	exit 1
fi
svn_repository="https://projects.coin-or.org/svn/CppAD"
# --------------------------------------------------------------------------
dd=`date +%d`
if [ "$dd" != '01' ]
then
	echo 'tag_month.sh: Must be run on the first day of the month'
	exit 1
fi
svn_yyyymmdd=`svn log $svn_repository/trunk --limit 1 | grep '^r[0-9]* *|' | \
	sed -e 's/^[^|]*|[^|]*| *\([0-9-]*\).*/\1/' -e 's|-||g'`
#
svn_hash=`svn log $svn_repository/trunk --limit 1 | \
	grep 'end *hash *code:' | sed -e 's|end *hash *code: *||'`
#
yyyy=`date +%Y`
svn_yyyy=`echo $svn_yyyymmdd | sed -e 's|^\(....\).*|\1|'`
if [ "$yyyy" != "$svn_yyyy" ]
then
	echo 'tag_month.sh: last change in svn trunk is for a different year.'
	echo "yyyy = $yyyy, svn_yyyy = $svn_yyyy"
	exit 1
fi
mm=`date +%m`
svn_mm=`echo $svn_yyyymmdd | sed -e 's|^....\(..\).*|\1|'`
if [ "$svn_mm" -ge "$mm" ]
then
	echo 'tag_month.sh: svn trunk has changes on or after this month.'
	echo "mm = $mm, svn_mm = $svn_mm"
	exit 1
fi
monthly_version="$yyyy$mm$dd"
# -----------------------------------------------------------------------------
# tag this version of the repository
if git tag --list | grep "$monthly_version"
then
	git tag -d $monthly_version
	git push --delete origin $monthly_version
fi
#
echo "git tag -a \\"
echo "-m \"corresponds $svn_repository/trunk on $monthly_version\" \\"
echo "$monthly_version $svn_hash"
git tag -a \
	-m "corresponds $svn_repository/trunk on $monthly_version" \
	$monthly_version $svn_hash
#
echo "git push origin $monthly_version"
git push origin $monthly_version
