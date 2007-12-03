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
# Some Example Spec Files. 
# http://www.rpm.org/RPM-HOWTO/build.html
# http://www.rpm.org/howto/thefight/
# http://www.rpm.org/max-rpm/s1-rpm-build-creating-spec-file.html
#
# Commands to Execute
# Put a copy of cppad-yyyymmdd.cpl.tgz in /usr/src/redhat/SOURCES
# Put a copy of cppad-yyymmdd.spec in /usr/src/redhat/SPECS
# Execute the command: sudo rpmbuild -ba --target=noarch cppad-yyyymmdd.spec
#
# RPMS Created:
# /usr/src/redhat/SRPMS/cppad-yyyymmdd-1.src.rpm
# /usr/src/redhat/RPMS/noarch/cppad-yyyymmdd-1.noarch.rpm
#
Name: cppad
Summary: A Package for Differentiation of C++ Algorithms
Version: yyyymmdd
Release: 1
License: CPL
Group: Math 
Source: http://www.coin-or.org/download/source/CppAD/cppad-yyyymmdd.cpl.tgz
URL: http://www.coin-or.org/CppAD/
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Packager: Brad Bell <bradbell@seanet.com>

%description
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, CppAD generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation can be found at
	http://en.wikipedia.org/wiki/Automatic_differentiation

# before extracting the source, remove the old build directory.
%prep
rm -rf $RPM_BUILD_ROOT

# uncompress and extract the source code and change into that directory
%setup -q 

# set prefix to /usr/local and create the test programs example/example and 
# test_more/test_more
%build
./configure --prefix=/usr/local  --with-Example --with-TestMore  --with-Documentation
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
# install include files to /usr/include/cppad
make install DESTDIR=$RPM_BUILD_ROOT

# list of files that are installed
%files
# default attributes, mode = same as after build, owner = root, group = root
%defattr(-,root,root)
%doc doc/*
/usr/local/include/cppad/*
/usr/local/share/doc
