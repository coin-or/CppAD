 test
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
# Bash script for building the CppAD distribution.
#
# Default values used for arguments to configure during this script.
# These defaults are development system dependent and can be changed.
ADOLC_DIR=$HOME/adolc_base
FADBAD_DIR=$HOME
BOOST_DIR=/usr/include/boost-1_33
#
# date currently in configure.ac
version=`grep "^ *AC_INIT(" configure.ac | \
	sed -e "s/.*, *\([0-9][0-9]-[0-9][0-9]-[0-9][0-9]\) *,.*/\1/"`
#
# version
#
if [ "$1" = "version" ] || [ "$1" = "all" ]
then
	echo "Build.sh version"
	#
	# Today's date in yy-mm-dd decimal digit format where 
	# yy is year in century, mm is month in year, dd is day in month.
	yy_mm_dd=`date +%g-%m-%d`
	#
	# change Autoconf version to today
	version=$yy_mm_dd
	#
	# configure.ac
	sed configure.ac > configure.ac.tmp \
	-e "s/(CppAD, [0-9][0-9]-[0-9][0-9]-[0-9][0-9] *,/(CppAD, $yy_mm_dd,/"
	diff configure.ac  configure.ac.tmp
	mv   configure.ac.tmp configure.ac
	#
	# AUTHORS
	sed AUTHORS > AUTHORS.tmp \
	-e "s/, 20[0-9][0-9]-[0-9][0-9]-[0-9][0-9] *,/, 20$yy_mm_dd,/"
	diff AUTHORS    AUTHORS.tmp
	mv   AUTHORS.tmp AUTHORS 
	#
	# update CppAD/config.h
	# even though gets overwritten when configure runs.
	sed CppAD/config.h > CppAD/config.tmp \
	-e "s/\"[0-9][0-9]-[0-9][0-9]-[0-9][0-9]\"/\"$yy_mm_dd\"/" \
	-e "s/ [0-9][0-9]-[0-9][0-9]-[0-9][0-9]\"/ $yy_mm_dd\"/"
	diff CppAD/config.h   CppAD/config.tmp
	mv   CppAD/config.tmp CppAD/config.h
	#
	for name in Doc.omh omh/InstallUnix.omh omh/InstallWindows.omh
	do
		sed $name > $name.tmp \
		-e "s/cppad-[0-9][0-9]-[0-9][0-9]-[0-9][0-9]/cppad-$yy_mm_dd/g"
		diff $name $name.tmp
		mv   $name.tmp $name
	done
	#
	# configure file is out of date so remove it
	if [ -e configure ]
	then
		rm configure
	fi
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# automake
#
if [ "$1" = "automake" ] || [ "$1" = "all" ]
then
	echo "Build.sh automake"
	#
	if [ -e configure ]
	then
		rm configure
	fi
	echo "---------------------------------------------------------"
	echo "If aclocal generates warning messages, run ./FixAclocal.sh"
	echo "aclocal"
	aclocal
	echo "---------------------------------------------------------"
	#
	echo "autoheader"
	autoheader
	#
	echo "autoconf"
	autoconf
	#
	echo "automake -add-missing"
	automake --add-missing
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# configure
#
if [ "$1" = "configure" ] || [ "$1" = "all" ]
then
	if [ "$1" = "configure" ] && [ "$2" == "test" ]
	then
		echo "Build.sh configure test"
	else
		echo "Build.sh configure"
	fi
	#
	TEST="--with-GetStarted"
	if [ "$1" = "configure" ] && [ "$2" == "test" ]
	then
		TEST="$TEST --with-Introduction"
		TEST="$TEST --with-Example"
		TEST="$TEST --with-TestMore"
		TEST="$TEST --with-Speed"
		TEST="$TEST --with-PrintFor"
		TEST="$TEST --with-SpeedExample"
		TEST="$TEST --with-profiling"
		if [ -e $ADOLC_DIR/include/adolc ]
		then
			TEST="$TEST ADOLC_DIR=$ADOLC_DIR"
		fi
		if [ -e $FADBAD_DIR/FADBAD++ ]
		then
			TEST="$TEST FADBAD_DIR=$FADBAD_DIR"
		fi
		if [ -e $BOOST_DIR/boost ]
		then
			TEST="$TEST BOOST_DIR=$BOOST_DIR"
		fi
	fi
	#
	echo "configure \\"
	echo "  $TEST   \\"
	echo "	CPP_ERROR_WARN=\"-Wall -ansi -pedantic-errors -std=c++98\""
	#
	./configure \
		$TEST   \
		CPP_ERROR_WARN="-Wall -ansi -pedantic-errors -std=c++98"
	#
	# Fix Makefile for what appears to be a bug in gzip under cygwin
	echo "FixMakefile.sh"
	./FixMakefile.sh
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# make
#
if [ "$1" = "make" ] || [ "$1" = "all" ]
then
	echo "Build.sh make"
	#
	echo "make"
	make
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# omhelp
#
if [ "$1" = "omhelp" ] || [ "$1" = "all" ]
then
	echo "Build.sh omhelp"
	#
	echo "RunOMhelp.sh Dev"
	if ! ./RunOMhelp.sh Dev
	then
		exit 1
	fi
	#
	echo "RunOMhelp.sh Doc"
	if ! ./RunOMhelp.sh Doc
	then
		exit 1
	fi
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# dist
#
if [ "$1" = "dist" ] || [ "$1" = "all" ]
then
	echo "Build.sh dist"
	#
	if [ -e cppad-$version ]
	then
		echo "rm -f -r cppad-$version"
		if ! rm -f -r cppad-$version
		then
			echo "Build: cannot remove old cppad-$version"
			exit 1
		fi
	fi
	for file in cppad-*.tgz cppad-*.zip
	do
		if [ -e $file ]
		then
			echo "rm $file"
			rm $file
		fi
	done
	#
	echo "make dist"
	make dist
	#
	if [ ! -e cppad-$version.tar.gz ]
	then
		echo "cppad-$version.tar.gz does not exist"
		echo "perhaps version is out of date"
		#
		exit 1
	fi
	# change *.tgz to *.cpl.tgz
	if ! mv cppad-$version.tar.gz cppad-$version.cpl.tgz
	then
		echo "cannot move cppad-$version.tar.gz to cppad-$version.tgz"
		exit 1
	fi
	#
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
if [ "$1" = "test" ] || ( [ "$1" = "all" ] && [ "$2" = "test" ] )
then
	#
	if [ -e cppad-$version ]
	then
		echo "rm -f -r cppad-$version"
		if ! rm -f -r cppad-$version
		then
			echo "Build: cannot remove old cppad-$version"
			exit 1
		fi
	fi
	#
	if [ -e "cppad-$version.cpl.tgz" ]
	then
		dir="."
	else
		if [ -e "Doc/cppad-$version.cpl.tgz" ]
		then
			dir="Doc"
		else
			echo "cannot find cppad-$version.cpl.tgz"
			exit 1
		fi
	fi
	echo "tar -xzf $dir/cppad-$version.cpl.tgz"
	tar -xzf $dir/cppad-$version.cpl.tgz
	#
	wd=`pwd`
	cd cppad-$version
	./Build.sh configure test
	make
	echo "creating $wd/Test.log"
	Example/Example           >  ../Test.log
	TestMore/TestMore         >> ../Test.log
	Introduction/Introduction >> ../Test.log
	Fadbad/Example            >> ../Test.log
	Adolc/Example             >> ../Test.log
	RunOMhelp.sh Doc
	cat OMhelp.Doc.log        >> ../Test.log
	if [ "$1" != "all" ]
	then
		exit 0
	fi
	# None of the "test" cases get past this point
	if [ "$2" = "test" ]
	then
		exit 0
	fi
fi
if [ "$1" = "gpl+dos" ] || [ "$1" = "all" ]
then
	# create GPL licensed version
	echo "GplLicense.sh"
	if ! ./GplLicense.sh
	then
		exit 1
	fi
	echo "./DosFormat.sh"
	if ! ./DosFormat.sh
	then
		exit 1
	fi
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
if [ "$1" = "move" ] || [ "$1" = "all" ] 
then
	# copy tarballs into Doc directory
	list="
		cppad-$version.cpl.tgz
		cppad-$version.gpl.tgz
		cppad-$version.cpl.zip
		cppad-$version.gpl.zip
	"
	for file in $list
	do
		echo "mv $file Doc/$file"
		if ! mv $file Doc/$file
		then
			echo "cannot move $file to Doc/$file"
			exit 1
		fi
	done
       if [ "$1" != "all" ]
       then
               exit 0
       fi
fi
if [ "$1" = "all" ]
then
	exit 0
fi
#
if [ "$1" = "" ]
then
	echo "usage: Build.sh option (where valid options are listed below)" 
else
	echo "$1 is not a valid option (valid options are listed below)"
fi
echo "option"
echo "------"
echo "version        update configure.ac and Doc.omh version number"
echo "automake       run aclocal,autoheader,autoconf,automake -> configure"
echo "configure      excludes --with-* except GetStarted and Introduction"
echo "configure test includes all the possible options except PREFIX_DIR"
echo "make           use make to build all of the requested targets"
echo "omhelp         build all the documentation in Doc & Dev directories"
echo "dist           create the distribution file cppad-version.cpl.tgz"
echo "test           unpack *.cpl.tgz, compile, run tests, result in Test.log"
echo "gpl+dos        create ./*.gpl.tgz, ./*.gpl.zip, and ./*.cpl.zip"
echo "move           move ./*.tgz and ./*.zip to Doc directory"
echo
echo "Build.sh all"
echo "This command will execute all the options in the order above with the"
echo "exception that \"configue test\" and \"test\" will be excluded."
echo
echo "Build.sh all test"
echo "This command will execute all the options in the order above"
echo "with the exception of \"configure test\", \"gpl+dos\" and \"move\"."
#
exit 1
