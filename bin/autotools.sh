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
# build script for use with autotools install
# -----------------------------------------------------------------------------
# prefix directories for the corresponding packages
#
# This test script no longer works with the current version of ADOLC
# ADOLC_DIR=$HOME/prefix/adolc
#
BOOST_DIR=/usr
CPPAD_DIR=$HOME/prefix/cppad
EIGEN_DIR=$HOME/prefix/eigen
FADBAD_DIR=$HOME/prefix/fadbad
IPOPT_DIR=$HOME/prefix/ipopt
# version type is one of "trunk" or "stable"
version_type="stable"
# -----------------------------------------------------------------------------
if [ $0 != "bin/autotools.sh" ]
then
	echo "bin/autotools.sh: must be executed in the directory that contians it"
	exit 1
fi
if [ "$2" != "" ]
then
	# when running multiple options, start by removing old log files
	touch junk.log
	list=`ls *.log`
	for log in $list
	do
		echo "rm $log"
		rm $log
	done
	#
	# run multiple options in order
     for option in $*
     do
		echo "=============================================================="
		echo "begin: bin/autotools.sh $option"
          bin/autotools.sh $option
     done
	echo "=============================================================="
     exit 0
fi
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	echo "mkdir build"
	mkdir build
fi
# -----------------------------------------------------------------------------
# Today's date in yyyy-mm-dd decimal digit format where
# yy is year in century, mm is month in year, dd is day in month.
yyyy_mm_dd=`date +%F`
#
# Version of cppad that corresponds to today.
if [ "$version_type" == "trunk" ]
then
	version=`bin/version.sh get`
else
	version=`grep '^ *AC_INIT(' configure.ac |
		sed -e 's/[^,]*, *\([^ ,]*\).*/\1/' -e 's|\[||' -e 's|\]||'`
	yyyy_mm_dd=`echo $version |
		sed -e 's|\..*||' -e 's|\(....\)\(..\)|\1-\2-|'`
fi
#
# Files are created by the configure command and copied to the source tree
configure_file_list="
	cppad/configure.hpp
"
# -----------------------------------------------------------------------------
# change version to current date
if [ "$1" = "version" ]
then
	echo 'bin/version.sh check'
	bin/version.sh check
	#
	echo "OK: bin/autotools.sh version"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "automake" ]
then
	#
	# check that autoconf and automake output are in original version
	makefile_in=`sed configure.ac \
	-n \
	-e '/END AC_CONFIG_FILES/,$d' \
	-e '1,/AC_CONFIG_FILES/d' \
	-e 's|/makefile$|&.in|' \
	-e '/\/makefile.in/p'`
	auto_output="
		depcomp
		install-sh
		missing
		configure
		config.guess
		config.sub
		$makefile_in
	"
	missing=""
	for name in $auto_output
	do
		if [ ! -e $name ]
		then
			if [ "$missing" != "" ]
			then
				missing="$missing, $name"
			else
				missing="$name"
			fi
		else
			# force remake of files
			rm "$name"
		fi
	done
	if [ "$missing" != "" ]
	then
		echo "The following files:"
		echo "	$missing"
		echo "are not in subversion repository."
		echo "Check them in when this command is done completes."
	fi
	#
	echo "aclocal"
	aclocal
	#
	echo "skipping libtoolize"
	# echo "libtoolize -c -f -i"
	# if ! libtoolize -c -f -i
	# then
	#	exit 1
	# fi
	#
	echo "autoconf"
	autoconf
	#
	echo "automake --add-missing"
	automake --add-missing
	#
	link_list="missing install-sh depcomp config.sub config.guess"
	for name in $link_list
	do
		if [ -h "$name" ]
		then
			echo "Converting $name from a link to a regular file"
			#
			echo "cp $name $name.$$"
			cp $name $name.$$
			#
			echo "mv $name.$$ $name"
			mv $name.$$ $name
		fi
	done
	#
	echo "OK: bin/autotools.sh automake"
	exit 0
fi
# -----------------------------------------------------------------------------
# configure
if [ "$1" == "configure" ]
then
	log_dir=`pwd`
	log_file="$1.log"
	#
	echo "cd build"
	cd build
	#
	dir_list="
		--prefix=$CPPAD_DIR
	"
	if [ -e $BOOST_DIR/include/boost ]
	then
		dir_list="$dir_list BOOST_DIR=$BOOST_DIR"
#_build_test_only:	if [ ! -e $EIGEN_DIR/include/Eigen ]
#_build_test_only:	then
#_build_test_only:		dir_list="$dir_list --with-boostvector"
#_build_test_only:	fi
	fi
	if [ -e $EIGEN_DIR/include/Eigen ]
	then
		dir_list="$dir_list
			EIGEN_DIR=$EIGEN_DIR"
#_build_test_only:	dir_list="$dir_list --with-eigenvector"
	fi
	if [ -e $FADBAD_DIR/include/FADBAD++ ]
	then
		dir_list="$dir_list
			FADBAD_DIR=$FADBAD_DIR"
	fi
	if [ -e $IPOPT_DIR/include/coin/IpIpoptApplication.hpp ]
	then
		dir_list="$dir_list
		IPOPT_DIR=$IPOPT_DIR"
	fi
	# Use =int (a signed type) to do more checking for
	# slicing from size_t to addr_t.
	special_types=""
#_build_test_only:	special_types="TAPE_ADDR_TYPE=int TAPE_ID_TYPE=int"
	#
	dir_list=`echo $dir_list | sed -e 's|\t\t*| |g'`
	cxx_flags="-Wall -ansi -pedantic-errors -std=c++11 -Wshadow"
	cxx_flags="$cxx_flags -isystem $EIGEN_DIR/include"
cat << EOF
../configure > $log_file \\
$dir_list \\
CXX_FLAGS=\"$cxx_flags\" \\
$special_types OPENMP_FLAGS=-fopenmp \\
--with-Documentation
EOF
	#
	../configure > $log_dir/$log_file \
		$dir_list \
		CXX_FLAGS="$cxx_flags" \
		$special_types OPENMP_FLAGS=-fopenmp \
		--with-Documentation
	#
	for file in $configure_file_list
	do
		echo "cp $file ../$file"
		cp $file ../$file
	done
	#
	echo "OK: bin/autotools.sh configure"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "dist" ]
then
	# ----------------------------------------------------------------------
	# Things to do in the original source directory
	# ----------------------------------------------------------------------
	echo "Only include the *.xml version of the documentation in distribution"
	if ! grep < doc.omh > /dev/null \
		'This comment is used to remove the table below'
	then
		echo "Missing comment expected in doc.omh"
		exit 1
	fi
	echo "sed -i.save doc.omh ..."
	sed -i.save doc.omh \
		-e '/This comment is used to remove the table below/,/$tend/d'
	#
	if [ -e doc ]
	then
		echo "rm -r doc"
		      rm -r doc
	fi
	#
	echo "bin/run_omhelp.sh xml"
	if ! bin/run_omhelp.sh xml
	then
		echo "mv doc.omh.save doc.omh"
		mv doc.omh.save doc.omh
		exit 1
	fi
	#
	echo "mv doc.omh.save doc.omh"
	      mv doc.omh.save doc.omh
	# No longer run these because tested when bin/package.sh is run.
	# Run automated checking of file names in original source directory
	# list="
	#	check_define.sh
	#	check_example.sh
	#	check_if.sh
	#	check_include_def.sh
	#	check_include_file.sh
	#	check_include_omh.sh
	#	check_makefile.sh
	#	check_op_code.sh
	#	check_replace.sh
	#	check_svn_id.sh
	#	check_verbatim.sh
	# "
	# for check in $list
	# do
	#	echo "bin/$check"
	#	      bin/$check
	# done
	# ----------------------------------------------------------------------
	# Things to do in the build directory
	# ----------------------------------------------------------------------
	echo "cd build"
	      cd build
	#
	if [ -e cppad-$version ]
	then
		echo "rm -rf cppad-$version"
		      rm -rf cppad-$version
	fi
	for file in cppad-*.tgz
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
	# change *.tgz to *.epl.tgz
	echo "mv cppad-$version.tar.gz cppad-$version.epl.tgz"
	      mv cppad-$version.tar.gz cppad-$version.epl.tgz
	#
	echo "OK: bin/autotools.sh dist"
	exit 0
fi
# -----------------------------------------------------------------------------
# omhelp comes after dist because dist only includes one help output
if [ "$1" = "omhelp" ]
then
	if ! grep < doc.omh > /dev/null \
		'This comment is used to remove the table below'
	then
		echo "doc.omh is missing a table."
		echo "Try re-running bin/autotools.sh configure."
	fi
	for flag in "printable" ""
	do
		# Run xml after htm so that index.htm points to cppad.xml
		# (see run_omhelp.sh).
		for ext in htm xml
		do
			echo "bin/run_omhelp.sh $ext $flag"
			      bin/run_omhelp.sh $ext $flag
		done
	done
	#
	echo "OK: bin/autotools.sh omhelp"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "doxygen" ]
then
	echo "bin/run_doxygen.sh"
	bin/run_doxygen.sh
	#
	echo "OK: bin/autotools.sh doxygen"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "gpl" ]
then
	# create GPL licensed version
	echo "bin/gpl_license.sh cppad-$version build build"
	bin/gpl_license.sh cppad-$version build build
	#
	echo "OK: bin/autotools.sh gpl"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "copy2doc" ]
then
	for ext in epl gpl
	do
		echo "cp build/cppad-$version.$ext.tgz doc/cppad-$version.$ext.tgz"
		cp build/cppad-$version.$ext.tgz doc/cppad-$version.$ext.tgz
	done
	#
	echo "cp -r doxydoc doc/doxydoc"
	cp -r doxydoc doc/doxydoc
	#
	echo "cp *.log doc"
	cp *.log doc
	#
	echo "OK: bin/autotools.sh copy2doc"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" == "all" ]
then
	list="
		version
		automake
		configure
		dist
		omhelp
		doxygen
		gpl
		copy2doc
	"
	if [ "$version_type" != "trunk" ]
	then
		# only use the help built during the bin/autotools.sh dist command
		list=`echo $list | sed -e 's|omhelp||'`
	fi
	echo "bin/autotools.sh $list"
	bin/autotools.sh $list
	echo "OK: bin/autotools.sh all"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "test" ]
then
	log_dir=`pwd`
	log_file="build_test.log"
	# --------------------------------------------------------------
	# Things to do in the distribution directory
	# --------------------------------------------------------------
	#
	# start log for this test
	echo "date > $log_file"
	      date > $log_dir/$log_file
	# ----------------------------------------------------------------------
	# Things to do in the build directory
	# ----------------------------------------------------------------------
	echo "cd build"
	echo "cd build" >> $log_dir/$log_file
	      cd build
	#
	# erase old distribution directory
	if [ -e cppad-$version ]
	then
		echo "rm -rf cppad-$version"
		echo "rm -rf cppad-$version" >> $log_dir/$log_file
		      rm -rf cppad-$version
	fi
	#
	# create distribution directory
	echo "tar -xzf cppad-$version.epl.tgz"
	echo "tar -xzf cppad-$version.epl.tgz" >> $log_dir/$log_file
	      tar -xzf cppad-$version.epl.tgz
	#
	# ----------------------------------------------------------------------
	# Things to do in the build/disribution directory
	# ----------------------------------------------------------------------
	echo "cd cppad-$version"
	echo "cd cppad-$version" >> $log_dir/$log_file
	      cd cppad-$version
	#
	# build_test_only configuration
	echo "sed -i -e 's|^#_build_test_only:||' bin/autotools.sh"
	sed -i -e 's|^#_build_test_only:||' bin/autotools.sh
	#
	echo "bin/autotools.sh configure >> $log_file"
	      bin/autotools.sh configure >> $log_dir/$log_file
	#
	# test user documentation
	echo "bin/run_omhelp.sh xml  >> $log_file"
	      bin/run_omhelp.sh xml  >> $log_dir/$log_file
	#
	# Developer documentation no longer works for autotools install
	# test developer documentation
	# echo "bin/autotools.sh doxygen   >> $log_file"
	#      bin/autotools.sh doxygen   >> $log_dir/$log_file
	#
	# ----------------------------------------------------------------------
	# Things to do in the build/disribution/build directory
	# ----------------------------------------------------------------------
	echo "cd build"
	echo "cd build" >> $log_dir/$log_file
	      cd build
	#
	dir=`pwd`
	echo "To see progress in the 'make test' log file use"
	echo "	../temp.sh ( OK | All | tail | follow | file )"
	cat << EOF > $log_dir/../temp.sh
#! /bin/bash -e
case "\$1" in

	OK)
	grep OK $dir/make_test.log
	exit 0
	;;

	All)
	grep All $dir/make_test.log
	exit 0
	;;

	tail)
	tail $dir/make_test.log
	exit 0
	;;

	follow)
	tail -f $dir/make_test.log
	exit 0
	;;

	file)
	echo "$dir/make_test.log"
	exit 0
	;;

	*)
	echo "usage: ../temp.sh option"
	echo "where option is one of following: OK, All, tail, follow, file."
	exit 1
esac
EOF
	chmod +x $log_dir/../temp.sh
	#
	# build and run all the tests
	echo "make test                >& make_test.log"
	      make test                >& make_test.log
	#
	echo "rm ../temp.sh"
	rm $log_dir/../temp.sh
	#
	echo "cat make_test.log        >> $log_file"
	      cat make_test.log        >> $log_dir/$log_file
	#
	# ignore warning in eigen (that has been reported)
	if grep ': *warning:' make_test.log
	then
		echo "There are warnings in $dir/make_test.log"
		exit 1
	fi
	# --------------------------------------------------------------------
	echo "cd ../../.."
	cd ../../..
	# end the build_test.log file with the date and time
	echo "date >> $log_file"
	      date >> $log_dir/$log_file
	#
	echo "No errors or warnings found; see build_test.log."
	#
	echo "OK: bin/autotools.sh test"
	exit 0
fi
# -----------------------------------------------------------------------------
# report bin/autotools.sh usage error
if [ "$1" != "" ]
then
     echo "$1 is not a valid option"
fi
#
if [ "$version_type" == "trunk" ]
then
	all_cases="run all the options above in order"
else
	all_cases="run all the options above in order with exception of omhelp"
fi
cat << EOF
usage: bin/autotools.sh option_1 option_2 ...

options                                                             requires
-------                                                             --------
version:  set version in AUTHORS, configure.ac, configure, ...
omhelp:   build all formats of user documentation in doc/*.
automake: run the tools required by autoconf and automake.
configure:run the configure script in the build directory.          automake
dist:     create the distribution file build/cppad-version.epl.tgz. configure
doxygen:  build developer documentation in doxydoc/*.               configure
gpl:      create build/*.gpl.zip and build/*.epl.zip.               dist
copy2doc: copy logs, tarballs & doxygen output into doc directory.  dist,doxygen

all:      $all_cases
test:     use tarball to make test and put result in build_test.log. dist
EOF
#
exit 1
