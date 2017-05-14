#! /bin/bash -e
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
if [ ! -e "bin/check_svn_id.sh" ]
then
	echo "bin/check_svn_id.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
list=`git status | sed -n \
        -e '/^[#\t ]*deleted:/p' \
        -e '/^[#\t ]*modified:/p' \
        -e '/^[#\t ]*both modified:/p' \
        -e '/^[#\t ]*renamed:/p' \
        -e '/^[#\t ]*new file:/p' | \
			sed -e 's/^.*: *//' -e 's/ -> /\n/' | \
			sed -e '/^makefile.in$/d' \
				-e '/^.gitignore$/d' \
				-e '/\/makefile.in$/d' \
				-e '/bin\/check_svn_id.sh$/d' \
				-e '/authors/d' \
				-e '/COPYING/d' |
                sort -u`
ok="yes"
for file in $list
do
	if [ -e $file ]
	then
		sed $file -e '/$Id.*\$/d' > check_svn_id.$$
		if ! diff $file check_svn_id.$$ > /dev/null
		then
			echo '----------------------------------------------------'
			echo "check_svn_id.sh: automatic changes to $file:"
			if diff $file check_svn_id.$$
			then
				echo 'bin/check_svn_id.sh: program error'
				rm check_svn_id.$$
				exit 1
			fi
			ok='no'
			if [ -x $file ]
			then
				mv check_svn_id.$$ $file
				chmod +x $file
			else
				mv check_svn_id.$$ $file
			fi
		fi
	fi
done
if [ -e check_svn_id.$$ ]
then
	rm check_svn_id.$$
fi
if [ "$ok" == 'no' ]
then
	exit 1
fi
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
