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
# Preform the following steps:
# 1. Put a copy of the file ../cppad-20071206.cpl.tgz in 
#    /usr/src/redhat/SOURCES
# 2. Put a copy of this file (cppad.spec) in 
#    /usr/src/redhat/SPECS
# 3. Change the value of _prefix from /usr/local to the directory you 
#    want to use for --prefix during the configure step of the install.    
%define _prefix  /usr/local
# 4. In the /usr/src/redhat/SPECS directory, execute the following command:
#    sudo rpmbuild -ba --target=noarch cppad.spec
#
# RPMS Created:
# /usr/src/redhat/SRPMS/cppad-20071206-1.src.rpm
# /usr/src/redhat/RPMS/noarch/cppad-20071206-1.noarch.rpm
#
# Note 1:
# There are no requirements because CppAD does not depend on any other packages.
#
# Note 2:
# There is not need for a debug version because CppAD does not install any
# object or binary files. It only installs include files.
#
# Note 3:
# The quotes in the comments below are from 
# http://fedoraproject.org/wiki/Packaging/Guidelines
# ============================================================================
Name: cppad
Version: 20071206
Release: 1
Summary: A Package for Differentiation of C++ Algorithms

Group: Math 
License: CPL
URL: http://www.coin-or.org/CppAD/
Source: http://www.coin-or.org/download/source/CppAD/cppad-20071206.cpl.tgz

# "The recommended values for the BuildRoot tag are ...
# %{_tmppath}/%{name}-%{version}-%{release}-root"
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

# "The Packager tag should not be used in spec files."
# Packager: Brad Bell, bradbell at seanet dot com

# ----------------------------------------------------------------------------
%description
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, CppAD generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation can be found at
	http://en.wikipedia.org/wiki/Automatic_differentiation

# ----------------------------------------------------------------------------
%prep
# Remove the old build directory (if it exists) and then
# uncompress and extract the source code and change into that directory
rm -rf $RPM_BUILD_ROOT
%setup -q 

# ----------------------------------------------------------------------------
%build
# Tell configure the prefix value, create test programs, and include docs,
# compile the tests programs, then run the tests and make sure they pass.
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

# ----------------------------------------------------------------------------
%install
# install include files to $RPM_BUILD_ROOT/%{_prefix}/cppad-20071206-1
if ! make install DESTDIR=$RPM_BUILD_ROOT
then
	echo "Error during make install DESTDIR=$RPM_BUILD_ROOT"
	exit 1
fi

# ----------------------------------------------------------------------------
%clean
# cleanup 
rm -rf $RPM_BUILD_ROOT

# ----------------------------------------------------------------------------
%files
# Default attributes, mode = same as after build, owner = root, group = root
# execute = same as after build. Install the doc and cppad directories.
%defattr(-,root,root,-)
%{_prefix}/include/cppad
%{_prefix}/share/doc

# ----------------------------------------------------------------------------
%changelog
* Mon Dec 03 2007 Brad Bell ( bradbell at seanet dot com )
- 20071203 is first version of CppAD that included RPM spec file.
