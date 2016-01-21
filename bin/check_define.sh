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
if [ $0 != "bin/check_define.sh" ]
then
	echo "bin/check_define.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Check '# define' versus '# undef' names and check for addon names"
echo '-----------------------------------------------------------------------'
cmd_list='define undef'
add_on_list='CG PY TMB MIXED'
file_list=`bin/ls_files.sh | sed -n \
	-e '/^cppad\/.*\.hpp$/p' \
	-e '/^cppad\/.*\.hpp\.in$/p'`
#
add_on_error='false'
for file in $file_list
do
	for cmd in $cmd_list
	do
		if [ ! -e $file.in ]
		then
			sed -n -e "/^# *$cmd /p" $file | \
				sed -e "s/^# *$cmd  *\([A-Za-z0-9_]*\).*/\1/" >> tmp.$$
		fi
		if [ "$cmd" == 'define' ]
		then
			sed -e '/_HPP$/d' -i tmp.$$
		fi
		sort -u tmp.$$ > $cmd.$$
		rm tmp.$$
	done
	for add_on in $add_on_list
	do
		if grep "CPPAD_${add_on}_" $file
		then
			add_on_error='true'
		fi
	done
done
if [ "$add_on_error" == 'true' ]
then
echo '-----------------------------------------------------------------------'
	echo 'check_define.sh: Error: add_on preprocessor symbol found'
	rm undef.$$
	rm define.$$
	exit 1
fi
if ! diff define.$$ undef.$$
then
	echo 'check_define.sh: Error: defines and undefs do not match'
	rm undef.$$
	rm define.$$
	exit 1
fi
echo '-----------------------------------------------------------------------'
rm undef.$$
rm define.$$
exit 0
