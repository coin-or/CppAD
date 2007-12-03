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
# RPM documentation
# http://www.rpm.org/max-rpm/
#
# Prefix for directories where CppAD will be installed, you may change this
%define _prefix  /usr/local
#
# Preform the following steps:
# 1. Put a copy of the file ../cppad-yyyymmdd.cpl.tgz in 
#    /usr/src/redhat/SOURCES
# 2. Put a copy of this file (cppad-yyyymmdd.spec) in 
#    /usr/src/redhat/SPECS
# 3. In the /usr/src/redhat/SPECS directory, execute the following command:
#    sudo rpmbuild -ba --target=noarch cppad-yyyymmdd.spec
#
# RPMS Created:
# /usr/src/redhat/SRPMS/cppad-yyyymmdd-1.src.rpm
# /usr/src/redhat/RPMS/noarch/cppad-yyyymmdd-1.noarch.rpm
#
# Note that CppAD has no requirements; i.e., there is no Requires field
# ----------------------------------------------------------------------------
Name: cppad
Summary: A Package for Differentiation of C++ Algorithms
Version: yyyymmdd
Release: 1
License: CPL
Group: Math 
Source: http://www.coin-or.org/download/source/CppAD/cppad-yyyymmdd.cpl.tgz
URL: http://www.coin-or.org/CppAD/
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Packager: Brad Bell, bradbell at seanet dot com

%description
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, CppAD generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation can be found at
	http://en.wikipedia.org/wiki/Automatic_differentiation

# Remove the old build directory before exactring the source code
%prep
rm -rf $RPM_BUILD_ROOT

# Uncompress and extract the source code and change into that directory
%setup -q 

# Set the prefix, create test programs 
# example/example and test_more/test_more, and run the tests
%build
./configure \
	--prefix=%{_prefix} \
	--with-Example \
	--with-TestMore  \
	--with-Documentation
make
for program in example/example test_more/test_more
do
	if ! ./$program
	then
		dir=`pwd`
		echo "Error detected by: $dir/$program"
		exit 1
	fi
done
#
# install include files to $RPM_BUILD_ROOT/%{_prefix}/cppad-yyyymmdd-1
if ! make install DESTDIR=$RPM_BUILD_ROOT
then
	echo "Error during make install DESTDIR=$RPM_BUILD_ROOT"
	exit 1
fi
#
# cleanup 
%clean
rm -rf $RPM_BUILD_ROOT
#
# List of files that are installed
%files
# default attributes, mode = same as after build, owner = root, group = root
%defattr(-,root,root)
%doc doc/*
%{_prefix}/include/cppad/*
%{_prefix}/share/doc
