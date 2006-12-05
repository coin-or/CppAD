#! /bin/bash
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
# experimental script for cygwin packaging of CppAD 
#
# version and release number
#
version=`grep "^ *AC_INIT(" configure.ac | \
        sed -e "s/.*, *\([0-9]\{8\}\) *,.*/\1/"`
release="1"
#
# Make cygwin_package directory
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
# extract the gpl version of the distribution 
#
file="doc/cppad-$version.gpl.tgz"
if [ ! -e $file ]
then
	echo "Must first use ./build.sh to create $file."
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
echo "cp -r cppad-$version cygwin_package/cppad-$version"
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
sdesc: "C++ algorithmic differentiation by operator overloading"
ldesc: "C++ algorithmic differentiation by operator overloading.
Forward and reverse mode as well as derivatives of arbitrary order 
are supported. See the home page below for more details:
   http://http://www.coin-or.org/CppAD/" 
category: Math
# Requires a standard compliant C++ compiler; e.g., gcc-g++.
requires: gcc-g++
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
CppAD for cygwin, version $version release $release README file.

DOC
We use DOC below for the directory that contains the CppAD documentation.
Binary distribution this version, DOC = $bin_doc
Source distribution this version, DOC = $src_doc
Web based most recent version,    DOC = $web_doc

DOC/*.htm:
Files in the DOC directory with with the .htm extension
represent mathematics using LaTex, for example see DOC/poly.htm.

DOC/*.xml:
Files in the DOC directory with with the .xml extension
represent mathematics using LaTex, for example see DOC/poly.xml.

DOC/(cppad.xml or cppad.htm):
Is the top (root) of the documentation tree for CppAD.

DOC/(getstarted.cpp.xml or getstarted.cpp.htm):
Contains an example for getting started using CppAD.

DOC/(introduction.xml or introduction.htm):
Contains an introduction by example to Algorithmic Differentiation.

DOC/(whats_new.xml or whats_new.htm):
Contains a list of changes and additions to CppAD in reverse order by date.

DOC/(installunix.xml or installunix.htm):
Contains documentation for the configure options which can be used to build
the examples and tests for CppAD (using the source distribution). 
EOF
#
# create the cppad-$version.patch file
#
diff -N -r -u -p ../cppad-$version cppad-$version > cppad-$version.patch 
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
# create the usr/share/doc/cppad-$version-$release directory
#
echo "mkdir usr/share/doc"
mkdir usr/share
mkdir usr/share/doc
#
echo "cp -r cppad-$version-$release/doc usr/share/doc/cppad-$version-$release"
if ! cp -r cppad-$version-$release/doc usr/share/doc/cppad-$version-$release
then
	exit 1
fi
chmod 644 usr/share/doc/cppad-$version-$release/*
chmod 755 usr/share/doc/cppad-$version-$release/
#
# create the usr/share/doc/Cygwin/cppad-$version-$release.README
#
mkdir "usr/share/doc/Cygwin"
if [ -d usr/share/doc/Cygwin ]
then
	echo "for an unknown reason, usr/share/doc/Cygwin already exists."
else
	mkdir usr/share/doc/Cygwin
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
# Clean up
# 
if ! rm -r cppad-$version-$release
then
	exit 1
fi
if ! rm -r usr
then
	exit 1
fi
if ! rm cppad-$version.patch
then
	exit 1
fi
#
# Done
#
exit 0
