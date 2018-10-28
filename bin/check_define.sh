#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != 'bin/check_define.sh' ]
then
	echo 'bin/check_define.sh: must be executed from its parent directory'
	exit 1
fi
for temp_file in check_define.1 check_define.2 check_define.3
do
	if [ -e $temp_file ]
	then
		echo "bin/check_define.sh: $temp_file exists"
		exit 0
	fi
done
# -----------------------------------------------------------------------------
echo 'Check '# define' versus '# undef' names and check for addon names'
echo '-----------------------------------------------------------------------'
file_list=`git ls-files '*.hpp' '*.in' |
	sed -n -e '/^cppad\//p'`
add_on_list='CG PY TMB MIXED'
#
# preprocessor symbol that may or may not be defined by user
echo 'CPPAD_DEBUG_AND_RELEASE' > check_define.1
#
for file in $file_list
do
		include_guard=`echo $file | sed \
			-e 's|\.in||' \
			-e 's|/|_|g' \
			-e 's|\.hpp|_hpp|' \
			| tr [a-z] [A-Z]
		`
		# define
		if [ ! -e $file.in ]
		then
			sed -n -e "/^# *define /p" $file | sed \
				-e "/^# *define *$include_guard/d" \
				-e "s/^# *define  *\([A-Za-z0-9_]*\).*/\1/" >> check_define.1
		fi
		# undef
		if [ ! -e $file.in ]
		then
			sed -n -e "/^# *undef /p" $file | sed \
				-e "s/^# *undef  *\([A-Za-z0-9_]*\).*/\1/" >> check_define.2
		fi
		# add_on
		for add_on in $add_on_list
		do
			if grep "CPPAD_${add_on}_" $file
			then
				add_on_error='true'
			fi
		done
done
# sort lists
for file in check_define.1 check_define.2
do
	sort -u $file > check_define.3
	mv check_define.3 $file
done
echo '-----------------------------------------------------------------------'
if [ "$add_on_error" == 'true' ]
then
	echo 'check_define.sh: Error: add_on preprocessor symbol found'
	rm check_define.1 check_define.2
	exit 1
fi
if ! diff check_define.1 check_define.2
then
	echo 'check_define.sh: Error: defines and undefs do not match'
	rm check_define.1 check_define.2
	exit 1
fi
rm check_define.1 check_define.2
echo 'check_define.sh: OK'
exit 0
