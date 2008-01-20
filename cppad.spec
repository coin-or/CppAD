# ----------------------------------------------------------------------------
# RPM source for this version of cppad.spec:
# http://www.seanet.com/~bradbell/cppad/cppad-%{version}-%{release}.fc7.src.rpm
# ----------------------------------------------------------------------------
# Known Problems: 
#
# The cppad-doc package has the following rpmlint error:
#         W: file-not-utf8 /usr/share/doc/cppad-%{version}/pmathmlcss.xsl
# This is the standard presentation MathML style sheet. Except for white
# space and a comment at the top, it is identical to
#         http://www.w3.org/Math/XSL/pmathmlcss.xsl
# (use wget and diff -b to see that this is true).
# ---------------------------------------------------------------------------- 
Name: cppad
Version: 20071229
Release: 3%{?dist}
Summary: %{name} base package (not installed)

Group: Development/Libraries 
License: GPLv2
URL: http://www.coin-or.org/CppAD/
Source: http://www.coin-or.org/CppAD/download/%{name}-%{version}.gpl.tgz
BuildArch: noarch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
There is no base %{name} package installation, 
only %{name}-devel and %{name}-doc sub-packages are installed.

# ---------------------------------------------------------------------------
%package devel
Summary: A Package for Differentiation of C++ Algorithms
Group: Development/Libraries

%description devel
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, %{name} generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation (AD) can be found at
Wiki: http://en.wikipedia.org/wiki/Automatic_differentiation

# ----------------------------------------------------------------------------
%package doc
Summary: Documentation for %{name}-devel
Group: Development/Libraries

%description doc
The %{name}-doc package contains the html and xml documentation for
the %{name}-devel package.

# ----------------------------------------------------------------------------
%prep
%setup -q 

# remove csetting of permissions in documentation destination directory
sed -i.stamp makefile.am -e '/chmod -R .* $(docdir_3)/d'
sed -i.stamp makefile.in -e '/chmod -R .* $(docdir_3)/d'

%build
%configure --with-Documentation \
           --with-Example \
           --with-Introduction \
           --with-TestMore \
           --with-Speed \
           --with-Documentation
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%check
example/example 
introduction/exp_apx/exp_apx
test_more/test_more
speed/cppad/cppad     correct 123
speed/double/double   correct 123
speed/example/example correct 123
speed/profile/profile correct 123

%clean
# cleanup 
rm -rf $RPM_BUILD_ROOT

%files devel
%defattr(-,root,root,-)
%doc COPYING ChangeLog AUTHORS uw_copy_040507.html
%{_includedir}/%{name}

%files doc
%defattr(-,root,root,-)
%{_docdir}/%{name}-%{version}

# ----------------------------------------------------------------------------
%changelog
* Wed Jan 09 2008  Brad Bell <bradbell at seanet dot com> 20071229-3
I mistakenly tried to make tag 20071229-2 in devel before commiting local 
changes. It appears tag was partially created, but not sure it is correct.
So I am bumping the version number. 

* Wed Jan 09 2008  Brad Bell <bradbell at seanet dot com> 20071229-2
Cygwin's verison of md5sum puts a <space><star> between the check sum
and the file name. Fedora build tools expect two spaces, so the star has
was changed to a space in the devel, F-7, and F-8 sources file.

* Sat Dec 29 2007  Brad Bell <bradbell at seanet dot com> 20071229-1
- Fix gpl_license.sh in upstream source (missed some special cases).

* Thu Dec 27 2007 Brad Bell <bradbell at seanet dot com> 20071225-2
- Fix spelling errors in this file and day of the week errors in %%changelog.
- Add ChangeLog, AUTHORS, uw_copy_040507.html to devel %%doc files.
 
* Tue Dec 25 2007 Brad Bell <bradbell at seanet dot com> 20071225-1
- %%Source points to newly created directory for archived versions cppad
- modify makefile.in so does not set permissions for documentation files

* Fri Dec 21 2007 Brad Bell <bradbell at seanet dot com> 20071221-1
- Added introduction/exp_apx/exp_apx to the list of correctness tests.
- Use %% to avoid macro expansion in %%changelog.
- Remove tabs from this spec file.
- Remove period from end of base package summary.
- Change upstream makefile.am so that it copies directories instead of files.

* Thu Dec 20 2007 Brad Bell <bradbell at seanet dot com> 20071208-2
- Increment release number each time a new spec file is uploaded.
- Use the commands %%configure, %%check.
- Remove the %%doc command.
- Use more macros, including %%{?_smp_mflags}, %%{_includedir}, %%{_docdir}.

* Thu Dec 20 2007 Brad Bell <bradbell at seanet dot com> 20071208-1
- Remove comments, except for those that are useful to a fedora reviewer. 
- Use different Summary and description for each sub-package.
- Use %%{?dist} in Release entry.
- Use %%(%%{__id_u} -n) in BuildRoot entry. 
- Use noarch in BuildArch entry.
- Move -rf $RPM_BUILD_ROOT from prep entry to install entry.
- Use macros where possible.

* Sat Dec 08 2007 Brad Bell <bradbell at seanet dot com> 20071208-1
- Fix all but one rpmlint warning (see Notes at beginning of this file).

* Mon Dec 03 2007 Brad Bell <bradbell at seanet dot com> 20071203-1
- first version of cppad that included RPM spec file.
