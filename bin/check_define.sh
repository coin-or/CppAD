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
if [ $0 != "bin/check_define.sh" ]
then
	echo "bin/check_define.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Difference between '# define' names and '# undef' names"
echo '-----------------------------------------------------------------------'
list=`bin/list_files.sh | sed -n \
	-e '/^cppad\/.*\.hpp$/p' \
	-e '/^cppad\/.*\.hpp\.in$/p'`
#
for cmd in define undef
do
	for file in $list
	do
		if [ ! -e $file.in ]
		then
			sed -n -e "/^# *$cmd /p" $file | \
				sed -e "s/^# *$cmd  *\([A-Za-z0-9_]*\).*/\1/" >> tmp.$$
		fi
	done
	if [ "$cmd" == 'define' ]
	then
		sed -e '/_HPP$/d' -i tmp.$$
	fi
	sort -u tmp.$$ > $cmd.$$
	rm tmp.$$
done
if ! diff define.$$ undef.$$
then
	echo "check_define.sh: exiting because defines and undefs do not match"
	rm undef.$$
	rm define.$$
	exit 1
fi
echo '-----------------------------------------------------------------------'
rm undef.$$
rm define.$$
exit 0
