# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Change date of subversion version of file to current date and then
# difference that with local version of file
#
# year, month, and date
yy_mm_dd=`date +%g-%m-%d`
#
if [ ! -e "$1" ]
then
	echo "diffToday file_name"
	exit
fi
svn cat $1 | sed > $1.tmp \
	-e "s/, 20[0-9][0-9]-[0-9][0-9]-[0-9][0-9] *,/, 20$yy_mm_dd,/" \
	-e "s/, [0-9][0-9]-[0-9][0-9]-[0-9][0-9] *,/, $yy_mm_dd,/" \
	-e "s/ [0-9][0-9]-[0-9][0-9]-[0-9][0-9]\"/ $yy_mm_dd\"/" \
	-e "s/\"[0-9][0-9]-[0-9][0-9]-[0-9][0-9]\"/\"$yy_mm_dd\"/" \
	-e "s/cppad-[0-9][0-9]-[0-9][0-9]-[0-9][0-9]/cppad-$yy_mm_dd/g"
#
echo "diff $1-local $1-subversion"
diff          $1      $1.tmp
rm                    $1.tmp


