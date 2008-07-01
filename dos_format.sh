# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
version=`grep "^ *AC_INIT(" configure.ac | \
        sed -e 's/[^,]*, *\([^ ,]*\).*/\1/'`
if [ -e DosFormat.log ]
then
	rm DosFormat.log
fi
#
# delete old version of *.cpl.zip and *.gpl.zip
for file in cppad-$version.cpl.zip cppad-$version.gpl.zip
do
	if [ -e $file ]
	then
        	echo "rm -f $file"
        	if ! rm -f $file
        	then
			echo "DosFormat: cannot remove old $file"
        		exit 1
		fi
	fi
done
for license in cpl gpl
do
	# delete old version of directory (if it exists)
	if [ -e cppad-$version ]
	then
		echo "rm -rf cppad-$version"
		if ! rm -rf cppad-$version
		then
			echo "DosFormat: cannot remove old cppad-$version"
			exit 1
		fi
	fi
	#
	# Convert this license version 
	if [ ! -e cppad-$version.$license.tgz ]
	then
		echo "DosFormat: cannot find cppad-$version.$license.tgz"
		exit 1
	fi
	#
	# extract from the *.$license.tgz file
	echo "tar -xzf cppad-$version.$license.tgz"
	tar -xzf cppad-$version.$license.tgz
	#
	# make sure can change into new directory 
	if ! cd cppad-$version
	then
	echo "DosFromat: cannot make new cppad-$version working directory"
	exit 1
	fi
	#
	list=`find . \
		\( -name '*.am'  \) -or \
		\( -name '*.ac'  \) -or \
		\( -name '*.sh' \) -or \
		\( -name '*.cpp' \) -or \
		\( -name '*.h'   \) -or \
		\( -name '*.hpp' \) -or \
		\( -name '*.omh' \) -or \
		\( -name '*.txt' \)`
	list="
		$list
		AUTHORS
		COPYING
		ChangeLog
		INSTALL
		NEWS
	"
	cd ..
	echo "DosFormat: converting file formats from unix to dos"
	for file in $list
	do
		ext=`echo $file | sed -e "s/.*\././"`
		file=`echo $file | sed -e 's|^\./||'`
		#
		unix2dos cppad-$version/$file >& DosFormat.log
		#
		if [ "$ext" = ".sh" ]
		then
			chmod +x cppad-$version/$file
		fi
	done
	#
	# Make sure that dates in certain files are older than the 
	# files converted and make sure they are in a certain time order.
	echo "DosFormat: ensuring proper time order in file dates"
	touch cppad-$version/aclocal.m4
	sleep 2
	touch cppad-$version/cppad/config.h.in
	sleep 2
	touch cppad-$version/makefile.in
	touch cppad-$version/*/makefile.in
	sleep 2
	touch cppad-$version/configure
	#
	echo "zip -q -r cppad-$version.$license.zip cppad-$version"
	zip -q -r cppad-$version.$license.zip cppad-$version
	#
done
