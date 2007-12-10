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
# CppAD Subversion Repository:
#	https://projects.coin-or.org/CppAD/browser
# CppAD Documentation in HTML or XHTML + MathML
#	http://www.coin-or.org/CppAD/Doc/cppad.htm
#	http://www.coin-or.org/CppAD/Doc/cppad.xml
# -----------------------------------------------------------------------------
# If this value is true, a set of validation tests are run to make sure that 
# CppAD works properly on the system where the rpmbuild command is executed.
%define _validation_testing_during_rpmbuild false
#
# Preform the following steps:
# 1. Put a copy of the file ../cppad-20071208.cpl.tgz in 
#        /usr/src/redhat/SOURCES
#    or get a copy of cppad-20071208-1.src.rpm and execute
#        sudo rpm --install cppad-20071208-1.src.rpm
# 2. Put a copy of this file (cppad.spec) in 
#        /usr/src/redhat/SPECS
# 3. In the /usr/src/redhat/SPECS directory, execute the following command:
#        sudo rpmbuild -ba --target=noarch cppad.spec
#
# RPMS Created:
# 1. /usr/src/redhat/SRPMS/cppad-20071208-1.src.rpm
# 2. /usr/src/redhat/RPMS/noarch/cppad-devel-20071208-1.noarch.rpm
# 3. /usr/src/redhat/RPMS/noarch/cppad-doc-20071208-1.noarch.rpm
#
# Note 1:
# There are no requirements because CppAD does not depend on any other packages.
#
# Note 2:
# There is no need for a debug or special architecture versions because CppAD 
# does not install any object or binary files. It is an include file library.
#
# Note 3:
# The is no base package cppad, only cppad-devel and cppad-doc.
#
# Note 4:
# The cppad-doc package has the following rpmlint error:
# W: file-not-utf8 /usr/share/doc/cppad-20071208/pmathmlcss.xsl
#    This is the standard presentation MathML style sheet. Except for white
#    space and a comment at the top, it is identical to
#         http://www.w3.org/Math/XSL/pmathmlcss.xsl
#    (use weget and diff -b to see that this is true).
#
# Note 5:
# A copy of the license is included in the files
#    /usr/share/doc/cppad-devel-20071208/COPYING
#    /usr/share/doc/cppad-20071208/license.htm
#    /usr/share/doc/cppad-20071208/license.xml
# ============================================================================
Name: cppad
Version: 20071208
Release: 1
Summary: A Package for Differentiation of C++ Algorithms

Group: Development/Libraries 
License: CPL
URL: http://www.coin-or.org/CppAD/

# To quote http://fedoraproject.org/wiki/Packaging/Guidelines
# "The recommended values for the BuildRoot tag are ...
# %{_tmppath}/%{name}-%{version}-%{release}-root"
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

# To quote http://fedoraproject.org/wiki/Packaging/Guidelines
# "The Packager tag should not be used in spec files."
# Packager: Brad Bell, bradbell at seanet dot com

# Each day a new CppAD tarball is created with the web address
#    http://www.coin-or.org/download/source/CppAD/cppad-yyyymmdd.lll.tgz
# where yyyy is the year, mm is the month, dd is the day, and lll is the
# license (either cpl or gpl).
#
# This version of cppad.spec corresponds to the following CppAD snapshot:
Source: http://www.coin-or.org/download/source/CppAD/cppad-20071208.cpl.tgz
# ---------------------------------------------------------------------------
%package devel
Summary: A Package for Differentiation of C++ Algorithms
Group: Development/Libraries

%package doc
Summary: A Package for Differentiation of C++ Algorithms
Group: Development/Libraries

# ----------------------------------------------------------------------------
%description
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, CppAD generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation can be found at
Wiki: http://en.wikipedia.org/wiki/Automatic_differentiation

%description devel
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, CppAD generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation can be found at
Wiki: http://en.wikipedia.org/wiki/Automatic_differentiation

%description doc
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, CppAD generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation can be found at
Wiki: http://en.wikipedia.org/wiki/Automatic_differentiation

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
# install files under $RPM_BUILD_ROOT/cppad-20071208-1/usr
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
%files devel
# Default attributes, mode = same as after build, owner = root, group = root
# execute = same as after build. Install the cppad include files.
%defattr(-,root,root,-)
%doc COPYING
/usr/include/cppad

%files doc
# Default attributes, mode = same as after build, owner = root, group = root
# execute = same as after build. Install the cppad documentation.
%defattr(-,root,root,-)
%doc /usr/share/doc/cppad-%{version}

# ----------------------------------------------------------------------------
%changelog
* Sat Dec 08 2007 Brad Bell ( bradbell at seanet dot com ) 20071208-1
- Fix all but one rpmlint warning (see Notes at beginning of this file).

* Mon Dec 03 2007 Brad Bell ( bradbell at seanet dot com ) 20071203-1
- first version of CppAD that included RPM spec file.
