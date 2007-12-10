# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Bradley M. Bell has given COIN-OR permission to use this script to generate 
# a distribution of CppAD that has "GNU General Public License Version 2"
# in place of "Common Public License Version 1.0." in all occurrences
# of the message above.
#
# date currently in configure.ac
version=`grep "^ *AC_INIT(" configure.ac | \
	sed -e "s/.*, \([0-9]\{8\}\) *,.*/\1/"`
#
# delete old version of *.gpl.tgz and *.gpl.zip
for file in cppad-$version.gpl.tgz cppad-$version.gpl.zip
do
	if [ -e $file ]
	then
        	echo "rm -f $file"
        	if ! rm -f $file
        	then
			echo "GplLicense: cannot remove old $file"
        		exit 1
		fi
	fi
done
#
# start from the *.cpl.tgz file
if [ ! -e cppad-$version.cpl.tgz ]
then
	echo "GplLicense: cannot find cppad-$version.cpl.tgz"
	exit 1
fi
#
# delete old version of directory (if it exists)
if [ -e cppad-$version ]
then
	echo "rm -f -r cppad-$version"
	rm -f -r cppad-$version
fi
if [ -e cppad-$version ]
then
	echo "GplLicense: cannot remove old cppad-$version directory"
	exit 1
fi
#
# extract from the *.cpl.tgz file
echo "tar -xzf cppad-$version.cpl.tgz"
tar -xzf cppad-$version.cpl.tgz
#
# make sure can change into new directory 
if ! cd cppad-$version
then
	echo "GplLicense: cannot make new cppad-$version working directory"
	exit 1
fi
#
list=`find . \
	\( -name '*.am'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.sh'  \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.in'  \) -or \
	\( -name '*.omh' \)`
#
# change back up to original directory (to be safe)
cd ..
#
echo "GplLicense: changing license from CPL to GPL"
for file in $list
do
	file=`echo $file | sed -e 's|^\./||'`
	#
	sed < cppad-$version/$file > GplLicense.tmp \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' 
	#
	mv GplLicense.tmp cppad-$version/$file
	#
	err="no"
	if ! grep "GNU General Public License Version 2" \
		cppad-$version/$file > /dev/null
	then
		err="yes"
		# Check for special case where automatically generated files
		# do not have any license statement.
		ext=`echo $file | sed -e 's|.*\.||'`
		if [ "$ext" = "in" ] 
		then
			if ! grep "Common Public License" \
				cppad-$version/$file > /dev/null
			then
				err="no"
			fi
		fi
	fi
	if [ "$err" = "yes" ]
	then
		echo "GplLicense: cannot change cppad-$version/$file license" 
		exit 1
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x cppad-$version/$file
	fi
done
#
# change cppad.spec from CPL to GPL
yyyymmdd=`date +%G%m%d`
sed < cppad.spec > cppad-$version/cppad.spec \
	-e "s/cppad-[0-9]\{8\}/cppad-$yyyymmdd/g" \
	-e "s/^Version: *[0-9]\{8\}/Version: $yyyymmdd/" \
	-e "s/CPL/GPL/g" \
	-e "s/License: *GPL *$/License: GPLv2/" \
	-e "s/\.cpl\./.gpl./g" \
	-e 's/Common Public License Version 1.0/GNU General Public License Version 2/'
#
# change the COPYING file
sed -n < COPYING > cppad-$version/COPYING \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' \
-e '/-\{70\}/,/-\{70\}/p'
cat gpl2.txt >> cppad-$version/COPYING
#
# change the file cpl1.0.txt to the file gpl2.txt
rm cppad-$version/cpl1.0.txt
cp gpl2.txt cppad-$version/gpl2.txt
#
# change occorances of cpl1.0.txt to gpl2.txt
sed < makefile.am > cppad-$version/makefile.am  \
	-e 's/cpl1.0.txt/gpl2.txt/'
#
sed < makefile.in > cppad-$version/makefile.in \
	-e 's/cpl1.0.txt/gpl2.txt/'
#
sed < omh/license.omh > cppad-$version/omh/license.omh \
	-e 's/$verbatim%cpl1.0.txt%$\$/$verbatim%gpl2.txt%$$/'
#
# Rerun omhelp to change documentation version of license from CPL to GPL 
cd cppad-$version
if ! ./run_omhelp.sh doc
then
	echo "Error running omhelp in gpl license version"
	exit 1
fi
if [ -e "doc/error.wrd" ]
then
	rm -rf doc/error.wrd
fi
cd ..
#
# create *.gpl.tgz file
echo "tar -czf cppad-$version.gpl.tgz cppad-$version"
tar -czf cppad-$version.gpl.tgz cppad-$version
