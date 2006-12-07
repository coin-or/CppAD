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
	ext=`echo $file | sed -e "s/.*\././"`
	file=`echo $file | sed -e 's|^\./||'`
	#
	sed < cppad-$version/$file > GplLicense.tmp \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' 

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
# change the COPYING file
sed -n < cppad-$version/COPYING > GplLicense.tmp \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' \
-e '/-\{70\}/,/-\{70\}/p'
#
cat gpl2.txt >> GplLicense.tmp
mv GplLicense.tmp cppad-$version/COPYING
#
# change the file cpl1.0.txt to the file gpl2.txt
rm cppad-$version/cpl1.0.txt
cp gpl2.txt cppad-$version/gpl2.txt
sed < cppad-$version/makefile.am > GplLicense.tmp \
	-e 's/cpl1.0.txt/gpl2.txt/'
mv GplLicense.tmp cppad-$version/makefile.am
sed < cppad-$version/makefile.in > GplLicense.tmp \
	-e 's/cpl1.0.txt/gpl2.txt/'
mv GplLicense.tmp cppad-$version/makefile.in
#
sed < cppad-$version/omh/license.omh > GplLicense.tmp \
	-e 's/$verbatim%cpl1.0.txt%$\$/$verbatim%gpl2.txt%$$/'
mv GplLicense.tmp cppad-$version/omh/license.omh
#
# Make sure that dates in certain files are older than the files converted 
# and make sure they are in a certain time order.
echo "GplLicense: ensuring certain time order in file dates"
touch cppad-$version/aclocal.m4
sleep 2
touch cppad-$version/cppad/config.h.in
sleep 2
touch cppad-$version/makefile.in
touch cppad-$version/*/makefile.in
sleep 2
touch cppad-$version/configure
#
# create *.gpl.tgz file
echo "tar -czf cppad-$version.gpl.tgz cppad-$version"
tar -czf cppad-$version.gpl.tgz cppad-$version
