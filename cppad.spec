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
# If this value is true, a set of validation tests are run to make sure that 
# CppAD works properly on the system where the rpmbuild command is executed.
%define _validation_testing_during_rpmbuild true
#
# Preform the following steps:
# 1. Put a copy of the file ../cppad-20071207.cpl.tgz in 
#    /usr/src/redhat/SOURCES
# 2. Put a copy of this file (cppad.spec) in 
#    /usr/src/redhat/SPECS
# 3. In the /usr/src/redhat/SPECS directory, execute the following command:
#    sudo rpmbuild -ba --target=noarch cppad.spec
#
# RPMS Created:
# /usr/src/redhat/SRPMS/cppad-20071207-1.src.rpm
# /usr/src/redhat/RPMS/noarch/cppad-20071207-1.noarch.rpm
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
#
# Note 4:
# This package has the following rpmlint errors:
# W: devel-file-in-non-devel-package .*\/usr\/include\/cppad.*\.h$
# W: devel-file-in-non-devel-package .*\/usr\/include\/cppad.*\.hpp$
#    CppAD is an include file library and hence its include files are
#    intended to be used by its users.
# W: file-not-utf8 /usr/share/doc/cppad-20071207/pmathmlcss.xsl
#    This is the standard presentation MathML style sheet.
# ============================================================================
Name: cppad
Version: 20071207
Release: 1
Summary: A Package for Differentiation of C++ Algorithms

Group: Development/Libraries 
License: CPL
URL: http://www.coin-or.org/CppAD/

# "The recommended values for the BuildRoot tag are ...
# %{_tmppath}/%{name}-%{version}-%{release}-root"
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

# "The Packager tag should not be used in spec files."
# Packager: Brad Bell, bradbell at seanet dot com

# This is the tarball file that contains this cppad.spec file
Source: http://www.coin-or.org/download/source/CppAD/cppad-20071207.cpl.tgz
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
if [ %{_validation_testing_during_rpmbuild} == "true" ]
then
	# Tell configure the prefix value, create test programs, 
	# include docs, compile the tests programs, then run the tests
	# and make sure they pass.
	./configure \
		--prefix=/usr \
		--with-Example \
		--with-TestMore \
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
else
	# Tell configure the prefix value and include docs
	./configure --prefix=/usr --with-Documentation
	make
fi
# ----------------------------------------------------------------------------
%install
# install files under $RPM_BUILD_ROOT/cppad-20071207-1/usr
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
/usr/include/cppad
/usr/share/doc/cppad-%{version}

# ----------------------------------------------------------------------------
%changelog
* Fri Dec 07 2007 Brad Bell ( bradbell at seanet dot com ) 20071207-1
- Remove option to relocate change the prefix directory
- and fix some other rpmlint errors.

* Mon Dec 03 2007 Brad Bell ( bradbell at seanet dot com ) 20071203-1
- first version of CppAD that included RPM spec file.
