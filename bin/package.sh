#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/package.sh" ]
then
	echo "bin/package.sh: must be executed from its parent directory"
	exit 1
fi
echo_exec() {
     echo $* 
     eval $*
}
top_srcdir=`pwd`
if [ -e package.log ]
then
	echo_exec rm package.log
fi
# ----------------------------------------------------------------------------
# Automated updates to source directory
#
# Use CMakeLists.txt to update version number in other files
version=`bin/version.sh get`
echo_exec bin/version.sh copy
#
# Update cppad/configure.hpp, example/test_one.sh, test_more/test_one.sh
if [ ! -d work ]
then
	echo_exec mkdir work
fi
echo_exec cd work
echo_exec cmake ..
echo_exec cd ..
# ----------------------------------------------------------------------------
# Run automated checking of file names in original source directory
# (check_include_omh.sh uses files built by cmake)
list="
	check_example.sh
	check_if_0.sh
	check_include_def.sh
	check_include_file.sh
	check_include_omh.sh
	check_makefile.sh
	check_op_code.sh
	check_svn_id.sh
	check_verbatim.sh
"
for check in $list 
do
	echo_exec bin/$check
done
# ----------------------------------------------------------------------------
# Check for doxygen errors
echo_exec bin/run_doxygen.sh
# ----------------------------------------------------------------------------
# Create the package directory
package_dir="work/cppad-$version"
if [ -e "$package_dir" ]
then
	echo_exec rm -r $package_dir
fi
if [ -e "$package_dir.epl.tgz" ]
then
	echo_exec rm $package_dir.epl.tgz
fi
echo_exec mkdir -p $package_dir
# -----------------------------------------------------------------------------
# Source file that are coppied to the package directory
file_list=`find . \
	\( -name '*.ac' \) -or \
	\( -name '*.am' \) -or \
	\( -name '*.c' \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.h' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.html' \) -or \
	\( -name '*.in' \) -or \
	\( -name '*.omh' \) -or \
	\( -name '*.pc' \) -or \
	\( -name '*.py' \) -or \
	\( -name '*.sed' \) -or \
	\( -name '*.sh' \) -or \
	\( -name '*.txt' \) | sed \
		-e '/\.\/work\//d' \
		-e '/\.\/build\//d' \
		-e '/\/gpl_license.sh$/d' \
		-e '/\/gpl-3.0.txt$/d' \
		-e '/\/junk.sh$/d' \
		-e 's|^\./||'`
other_files="
	AUTHORS 
	ChangeLog 
	configure 
	COPYING 
	depcomp
	INSTALL 
	NEWS 
	README 
"
#
# Copy the files, creating sub-directories when necessary
echo "copy source files to work/$package_dir >> package.log"
for file in $file_list $other_files
do
	sub_dir=`echo $file | sed -e 's|\(.*\)/[^/]*$|\1|'`
	if [ "$sub_dir" != "$file" ]
	then
		if [ ! -e "$package_dir/$sub_dir" ]
		then
			echo "mkdir -p $package_dir/$sub_dir" >> package.log
			mkdir -p $package_dir/$sub_dir
		fi
	fi
	echo "cp $file $package_dir/$file" >> package.log
	cp $file $package_dir/$file
done
# ----------------------------------------------------------------------------
# build the xml version of documentation for this distribution
echo_exec cd $package_dir
#
# Only include the *.xml verison of the documentation in distribution
# So remove the table at the top (but save the original doc.omh file).
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
# This command creates omhelp.xml.log in current directory (and says so)
echo "bin/run_omhelp.sh xml"
if ! bin/run_omhelp.sh xml
then
	mv omhelp.xml.log ../..
	echo_exec mv doc.omh.save doc.omh
	exit 1
fi
# Move the log to the directory where the package.sh command was executed
mv omhelp.xml.log ../..
#
echo_exec mv doc.omh.save doc.omh
# ----------------------------------------------------------------------------
# change back to the package parent directory and create the tarball
echo_exec cd ..
echo_exec tar -czf cppad-$version.epl.tgz cppad-$version
# ----------------------------------------------------------------------------
# create gpl version of package
echo_exec cd ..
echo_exec bin/gpl_license.sh cppad-$version work work
