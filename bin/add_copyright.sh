#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/add_copyright.sh" ]
then
	echo "bin/add_copyright.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
if [ "$1" == "" ]
then
	echo "./add_copyright.sh: file_name"
	exit 1
fi
file_name="$1"
ext=`echo $file_name | sed -e 's/.*\.//'`
if [ "$ext" == "" ]
then
	echo "./add_copyright.sh: file_name does not have an extension"
	exit 1
fi
#
echo "write copyright message to bin/add_copyright.$$"
case $ext in
	cpp | hpp | omh)
	cat << EOF  > bin/add_copyright.$$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
EOF
	;;

	sh)
	cat << EOF  > bin/add_copyright.$$
#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
EOF
	;;

	*)
	echo bin/"add_copyright.sh: extension $ext is not yet supported"
	exit 1
esac
#
echo "cat $file_name >> bin/add_copyright.$$"
cat $file_name >> bin/add_copyright.$$
#
echo "mv $file_name ~/trash"
mv $file_name ~/trash
#
echo "mv bin/add_copyright.$$ $file_name"
mv bin/add_copyright.$$ $file_name
