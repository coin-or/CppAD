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
if [ ! -e "bin/check_svn_id.sh" ]
then
	echo "bin/check_svn_id.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "Checking for \$Id.*\$ in beginning of source code"
echo "-------------------------------------------------------"
ok="yes"
list=`bin/list_files.sh | sed -n \
	-e '/^gpl-3.0.txt$/d' \
	-e '/^epl-v10.txt$/d' \
	-e '/cppad\/local\/config.h.in$/d' \
	-e '/^makefile.in$/d' \
	-e '/^svn_commit.sh$/d' \
	-e '/^git_commit.sh$/d' \
	-e '/\/makefile.in$/d'  \
	-e '/\.hpp$/p' \
	-e '/\.cpp$/p' \
	-e '/\.omh$/p' \
	-e '/\.txt$/p' \
	-e '/\.sh$/p'  \
	-e '/\.in$/p'  \
	-e '/\.am$/p'`
#
for file in $list
do
	# deprecated link files have just one line
	lines=`cat $file | wc -l`
	if [ "$lines" != 1 ]
	then
		if ! head -2 $file | grep '$Id.*\$' > /dev/null
		then
			echo "$file does not have '\$Id.*\$' in first two lines"
			ok="no"
		fi
	fi
done
echo "-------------------------------------------------------"
if [ "$ok" = "no" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
