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
if [ ! -e "bin/cygwin_package.sh" ]
then
	echo "bin/cygwin_package.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# experimental script for cygwin packaging of CppAD
#
# version and release number
#
# begin fresh cygwin_package directory
#
if [ -e cygwin_package ]
then
	echo "rm -r cygwin_package"
	if ! rm -r cygwin_package
	then
		exit 1
	fi
fi
#
# determine the most recent version in the Download directory
#
version=`ls /cygdrive/c/Download/cppad-20*.gpl.tgz |
	sed -n -e 's|.*/||' -e 's|cppad-||' -e 's|.gpl.tgz||' -e '$,$p'`
if [ "$version" == 20061208 ]
then
        echo "change: f'(3) -> y'(3)"
	echo "change: example/get_started -> get_started"
	echo "change: get_started.cpp.htm -> getstarted.cpp.htm"
	echo "change: get_started.cpp.xml -> getstarted.cpp.xml"
fi
release="1"
if [ ! -e "/cygdrive/c/Download/cppad-$version.gpl.tgz" ]
then
	echo "Error determining the version number, cannot find"
	echo "/cygdrive/c/Download/cppad-$version.gpl.tgz"
	exit 1
fi
#
# extract the gpl version of the distribution
#
file="/cygdrive/c/Download/cppad-$version.gpl.tgz"
if [ ! -e $file ]
then
	exit 1
fi
if [ -e cppad-$version ]
then
	echo "rm -r cppad-$version"
	if ! rm -r cppad-$version
	then
		echo "Cannot remove old cppad-$version directory."
	fi
fi
echo "tar -xzf $file"
if ! tar -xzf $file
then
	echo "Cannot create new cppad-$version directory."
        exit 1
fi
#
# make a copy of the distribution in the cygwin_package sub-directory
#
echo "mkdir cygwin_package"
mkdir cygwin_package
echo "cp -r cppad-$version cygwin_package/cppad-$version-$release"
if ! cp -r cppad-$version cygwin_package/cppad-$version-$release
then
	exit 1
fi
#
# change into the cygwin_package sub-directory and stay there
#
echo "cd cygwin_package"
if ! cd cygwin_package
then
	exit 1
fi
#
# Create the setup.hint file ----------------------------------------------
#
cat > setup.hint << EOF
# To test the source or binary distribution, copy the source distribution file
#	cppad-$version-$release/example/get_started/get_started.cpp
# to a temporary directory and execute the commands
#	g++ get_started.cpp -o get_started.exe
#	./get_started
# the resulting output should be
#	f'(3) computed by CppAD = 142
#
# Requires a standard compliant C++ compiler; e.g., gcc-g++.
requires: gcc-g++
#
category: Math
#
sdesc: "C++ algorithmic differentiation by operator overloading"
#
ldesc: "C++ algorithmic differentiation by operator overloading.
Forward and reverse mode as well as derivatives of arbitrary order
are supported. See the documentation for this version
    ( /usr/share/doc/cppad-$version-$release/cppad.htm )
or the CppAD home page
    ( http://http://www.coin-or.org/CppAD/ )
for more details."
EOF
#
# Create the source distribution ------------------------------------------
#
# Create the cygwin README file
#
bin_doc="/usr/share/doc/cppad-$version-$release"
src_doc="/usr/src/cppad-$version-$release/doc"
web_doc="http://www.coin-or.org/CppAD/Doc"
readme_file="cppad-$version-$release/README"
#
echo "create: $readme_file"
#
cat > $readme_file << EOF
CppAD for cygwin version number $version release number $release.

http://www.coin-or.org/CppAD
This is the CppAD home page.

cppad@list.coin-or.org
This mailing list is used for CppAD discussion and bug reporting. You must
first join the mailing list using its general information page
        http://list.coin-or.org/mailman/listinfo/CppAD
(this avoids having spam on the mailing list).

DOC
We use DOC below for the directories that contains the CppAD documentation.
Source distribution this version,         DOC = cppad-$version-$release/doc
Binary this version (post install),       DOC = $bin_doc
Source this version (post install),       DOC = $src_doc
The documentation of most recent version, DOC = $web_doc

DOC/*.htm
Files in the DOC directory with with the .htm extension
represent mathematics using LaTex; for example see DOC/poly.htm.

DOC/*.xml
Files in the DOC directory with with the .xml extension
represent mathematics using LaTex; for example see DOC/poly.xml.

DOC/(cppad.xml or cppad.htm)
Is the top (root) of the documentation tree for CppAD.

DOC/(get_started.cpp.xml or get_started.cpp.htm)
Contains an example for getting started using CppAD.

DOC/(introduction.xml or introduction.htm)
Contains an introduction by example to Algorithmic Differentiation.

DOC/(whats_new.xml or whats_new.htm)
Contains a list of changes and additions to CppAD in reverse order by date.

DOC/(installunix.xml or installunix.htm)
Contains documentation for the configure options which can be used to build
the examples and tests for CppAD (using the source distribution).
EOF
#
# change configure.ac
#
echo "Begin: modify configure.ac and files that depend on it."
echo "sed < cppad-$version-$release/configure.ac > configure.ac \\"
echo "	-e '/dnl cygwin begin delete:/,/dnl cygwin end delete:/d' \\"
echo "	-e 's/dnl cygwin replace: *//'"
#
if ! sed < cppad-$version-$release/configure.ac > configure.ac \
	-e '/dnl cygwin begin delete:/,/dnl cygwin end delete:/d' \
	-e 's/dnl cygwin replace: *//'
then
	exit 1
fi
#
if ! mv configure.ac cppad-$version-$release/configure.ac
then
	exit 1
fi
if ! cd cppad-$version-$release
then
	exit 1
fi
if ! aclocal
then
	exit 1
fi
if ! autoheader
then
	exit 1
fi
if ! autoconf
then
	exit 1
fi
if ! automake --add-missing
then
	exit 1
fi
# directory created by commands above and not in distribution
echo "rm -r autom4te.cache"
if ! rm -r autom4te.cache
then
	exit 1
fi
if ! cd ..
then
	exit 1
fi
echo "End: modify configure.ac and files that depend on it."
#
# create the cppad-$version.patch file
#
diff -N -r -u -p \
	../cppad-$version cppad-$version-$release > cppad-$version.patch
#
# create the cppad-$version-$release/CYGWIN-PATCHES sub-directory
#
echo "mkdir cppad-$version-$release/CYGWIN-PATCHES"
if ! mkdir cppad-$version-$release/CYGWIN-PATCHES
then
	exit 1
fi
if ! cp $readme_file cppad-$version-$release/CYGWIN-PATCHES/cppad.README
then
	exit 1
fi
if ! cp setup.hint cppad-$version-$release/CYGWIN-PATCHES
then
	exit 1
fi
if ! cp cppad-$version.patch cppad-$version-$release/CYGWIN-PATCHES
then
	exit 1
fi
#
# create cppad-$version-$release-src.tar.bz2
#
echo "tar -cf cppad-$version-$release-src.tar cppad-$version-$release"
if ! tar -cf cppad-$version-$release-src.tar cppad-$version-$release
then
	exit 1
fi
echo "bzip2 -z -f cppad-$version-$release-src.tar"
if ! bzip2 -z -f cppad-$version-$release-src.tar
then
	exit 1
fi
#
# Create the binary distribution ------------------------------------------
#
# create the usr/include/cppad directory
#
echo "mkdir usr/include"
mkdir usr
mkdir usr/include
#
echo "cp -r cppad-$version-$release/cppad usr/include/cppad"
if ! cp -r cppad-$version-$release/cppad usr/include/cppad
then
	exit 1
fi
chmod 644 usr/include/cppad/*.hpp
chmod 644 usr/include/cppad/local/*.hpp
chmod 755 usr/include/cppad
chmod 755 usr/include/cppad/local
#
# create the usr/share/doc/cppad-$version directory
#
echo "mkdir usr/share/doc"
mkdir usr/share
mkdir usr/share/doc
#
echo "cp -r cppad-$version-$release/doc usr/share/doc/cppad-$version"
if ! cp -r cppad-$version-$release/doc usr/share/doc/cppad-$version
then
	exit 1
fi
chmod 644 usr/share/doc/cppad-$version/*
chmod 755 usr/share/doc/cppad-$version
#
# create the usr/share/doc/Cygwin/cppad-$version-$release.README
#
echo "mkdir usr/share/doc/Cygwin"
if ! mkdir usr/share/doc/Cygwin
then
	exit
fi
if ! cp $readme_file usr/share/doc/Cygwin/cppad-$version-$release.README
then
	exit 1
fi
#
# create cppad-$version-$release.tar.bz2
#
echo "tar -cf cppad-$version-$release.tar usr"
if ! tar -cf cppad-$version-$release.tar usr
then
	exit 1
fi
echo "bzip2 -z -f cppad-$version-$release.tar"
if ! bzip2 -z -f cppad-$version-$release.tar
then
	exit 1
fi
#
# Test source install --------------------------------------------------------
#
echo "Begin: test if source install"
if [ -e /usr/include/cppad ]
then
	echo "rm -r /usr/include/cppad"
	if ! rm -r /usr/include/cppad
	then
		exit 1
	fi
fi
if [ -e /usr/share/doc/cppad-* ]
then
	echo "rm -rf /usr/share/doc/cppad-*"
	if ! rm -rf /usr/share/doc/cppad-*
	then
		exit 1
	fi
fi
mkdir test_src
echo "cd test_src"
if ! cd test_src
then
	exit 1
fi
echo "cp ../cppad-$version-$release-src.tar.bz2 ."
if ! cp ../cppad-$version-$release-src.tar.bz2 .
then
	exit 1
fi
echo "bunzip2 cppad-$version-$release-src.tar.bz2"
if ! bunzip2 cppad-$version-$release-src.tar.bz2
then
	exit 1
fi
echo "tar -xf cppad-$version-$release-src.tar"
if ! tar -xf cppad-$version-$release-src.tar
then
	exit 1
fi
echo "cd cppad-$version-$release"
if ! cd cppad-$version-$release
then
	exit 1
fi
echo "./configure"
if ! ./configure
then
	exit 1
fi
echo "make install"
if ! make install
then
	exit 1
fi
echo "cd ../.."
if ! cd ../..
then
	exit 1
fi
dir="test_src/cppad-$version-$release/example/get_started"
echo "cp $dir/get_started.cpp ."
if ! cp $dir/get_started.cpp .
then
	exit 1
fi
if ! g++ get_started.cpp -o get_started.exe
then
	exit 1
fi
echo "./get_started > get_started.out"
./get_started       > get_started.out
echo "f'(3) computed by CppAD = 142"  > get_started.chk
if ! diff get_started.out get_started.chk
then
	echo "error in get_started output"
	exit 1
fi
file_list="
	cppad
	get_started.cpp
	introduction
	whats_new
	installunix
"
ext_list="
	htm
	xml
"
for file in $file_list
do
	for ext in $ext_list
	do
		full_name=/usr/share/doc/cppad-$version/$file.$ext
		if [ ! -e "$full_name" ]
		then
			echo "cannot find $full_name"
			exit 1
		fi
	done
done
echo "End: test if source install"
#
# Test binary install --------------------------------------------------------
#
echo "Begin: test if binary install"
if [ -e /usr/include/cppad ]
then
	echo "rm -r /usr/include/cppad"
	if ! rm -r /usr/include/cppad
	then
		exit 1
	fi
fi
if [ -e /usr/share/doc/cppad-* ]
then
	echo "rm -rf /usr/share/doc/cppad-*"
	if ! rm -rf /usr/share/doc/cppad-*
	then
		exit 1
	fi
fi
mkdir test_bin
echo "cd test_bin"
if ! cd test_bin
then
	exit 1
fi
echo "cp ../cppad-$version-$release.tar.bz2 ."
if ! cp ../cppad-$version-$release.tar.bz2 .
then
	exit 1
fi
echo "bunzip2 cppad-$version-$release.tar.bz2"
if ! bunzip2 cppad-$version-$release.tar.bz2
then
	exit 1
fi
echo "tar -xf cppad-$version-$release.tar"
if ! tar -xf cppad-$version-$release.tar
then
	exit 1
fi
echo "cp -r usr/include/cppad /usr/include/cppad"
if ! cp -r usr/include/cppad /usr/include/cppad
then
	exit 1
fi
echo "cp -r usr/share/doc/cppad-$version /usr/share/doc/cppad-$version"
if ! cp -r usr/share/doc/cppad-$version /usr/share/doc/cppad-$version
then
	exit 1
fi
echo "cd .."
if ! cd ..
then
	exit 1
fi
if ! rm get_started.exe
then
	exit 1
fi
if ! g++ get_started.cpp -o get_started.exe
then
	exit 1
fi
echo "./get_started > get_started.out"
./get_started       > get_started.out
echo "f'(3) computed by CppAD = 142"  > get_started.chk
if ! diff get_started.out get_started.chk
then
	echo "error in get_started output"
	exit 1
fi
file_list="
	cppad
	get_started.cpp
	introduction
	whats_new
	installunix
"
ext_list="
	htm
	xml
"
for file in $file_list
do
	for ext in $ext_list
	do
		full_name=/usr/share/doc/cppad-$version/$file.$ext
		if [ ! -e "$full_name" ]
		then
			echo "cannot find $full_name"
			exit 1
		fi
	done
done
echo "End: test if binary install"
#
# Clean up ------------------------------------------------------------------
#
echo "cd .."
if ! cd ..
then
	exit 1
fi
list="
	cppad-$version-$release
	usr
	cppad-$version.patch
	test_src
	test_bin
"
for dir in $list
do
	echo "rm -r cygwin_package/$dir"
	if ! rm -r cygwin_package/$dir
	then
		exit 1
	fi
done
echo "rm cygwin_package/get_started.*"
if ! rm cygwin_package/get_started.*
then
	exit 1
fi
#
# ----------------------------------------------------------------------------
# Done
echo bin/"cygwin_package.sh: passed all its tests"
exit 0
