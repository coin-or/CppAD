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
if [ ! -e "bin/check_verbatim.sh" ]
then
	echo "bin/check_verbatim.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# Make sure that OMhelp verbatim commands referr to same file as command
echo "Checking that OMhelp verbatim commands include from file they appear in."
echo "----------------------------------------------------------------------"
list=`bin/list_files.sh | sed -n \
	-e '/\.c$/p' \
	-e '/\.cpp$/p' \
	-e '/\.hpp$/p' \
	-e '/\.omh$/p' \
	-e '/\.txt$/p' \
	-e '/\.am$/p'`
different="no"
for file in $list
do
	line=`sed -n -e '/$verbatim[^a-z]/p' $file`
	if [ "$line" != '' ]
	then
		reference=`echo $line | sed -e 's|$verbatim%\([^%]*\)%.*|\1|'`
		#
		file_root=`echo $file | sed \
			-e 's|.*/||' -e 's|_hpp\.omh||' -e 's|\.[^.]*$||'`
		#
		ref_root=`echo $reference | sed \
			-e 's|.*/||' -e 's|\.[^.]*$||'`
		#
		if [ "$file_root" != "$ref_root" ]
		then
			# special cases
			ok='false'
			if [ "$file_root" == 'cond_exp' ] && [ "$ref_root" == 'atan2' ]
			then
				ok='true'
			fi
			if [ "$file_root" == 'license' ] && [ "$ref_root" == 'epl-v10' ]
			then
				ok='true'
			fi
			if [ "$file_root"=='base_require' ] && [ "$ref_root"=='zdouble' ]
			then
				ok='true'
			fi
			#
			if [ "$ok" == 'false' ]
			then
				echo "\$verbatim in $file references $reference"
				different="yes"
			fi
		fi
	fi
done
echo "-------------------------------------------------------------------"
if [ $different = "yes" ]
then
	echo "Error: nothing should be between the two dashed lines above"
	exit 1
else
	echo "Ok: nothing is between the two dashed lines above"
	exit 0
fi
