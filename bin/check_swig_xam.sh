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
if [ $0 != "bin/check_swig_xam.sh" ]
then
	echo "bin/check_swig_xam.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
cd swig
list=`ls xam/*.m4 | sed -e 's|^xam/||' -e '/junk.m4$/d' -e 's|\.m4$||'`
ok='yes'
declare -A ext
ext['octave']='m'
ext['perl']='pm'
ext['python']='py'
for name in $list
do
	for lang in octave perl python
	do
		lang_file="$lang/$name.${ext[${lang}]}"
		if [ ! -e "$lang_file" ]
		then
			touch $lang_file
		fi
		m4 $lang.m4 xam/$name.m4 > check_swig_xam.$$
		if diff $lang_file check_swig_xam.$$ > /dev/null
		then
			rm check_swig_xam.$$
		else
			if [ "$ok" == 'yes' ]
			then
				echo '---------------------------------------------------------'
			fi
			mv check_swig_xam.$$ $lang_file
			echo "swig/$lang_file changed."
			ok='no'
		fi
	done
done
if [ "$ok" != 'yes' ]
then
	echo 'check_swig_xam.sh: changed some language specific files'
	exit 1
fi
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
