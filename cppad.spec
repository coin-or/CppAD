# Known rpmlint warnings: 
#
# cppad.spec: spelling-error Sumary(en_US) devel:
# Although not in the en_US dictionary, 'devel' is understood by Fedora users.
#
# cppad.spec: rpm-buildroot-usage
# The %%prep section of this spec file contains a reference to %%buildroot.
# This is used to read (not write) information in %%buildroot%%includdir
# (using g++ -I %%buildroot%%includedir ). Thus it is not a violation of
# http://fedoraproject.org/wiki/Packaging:Guidelines#
#      Scriplets_are_only_allowed_to_write_in_certain_directories
#
# cppad-20110101.2.gpl.tgz: strange-permission 0444L
# The premissions are for readonly, which seems fine in this case.
# 
# %%{_docdir}/%%{name}-%%{version}/pmathmlcss.xsl: file-not-utf8:
# The file pmathmlcss.xsl is the standard presentation MathML style sheet.
# Except for white space and a comment at the top, it is identical to
#     http://www.w3.org/Math/XSL/pmathmlcss.xsl
# (use wget and diff -b to see that this is true).
#
# ---------------------------------------------------------------------------- 

Name: cppad
Version: 20110101.2
Release: 3%{?dist}
Summary: C++ Algorithmic Differentiation (AD), %{name}-devel and %{name}-doc

Group: Development/Libraries 
License: GPLv2
URL: http://www.coin-or.org/CppAD/
Source: http://www.coin-or.org/download/source/CppAD/%{name}-%{version}.gpl.tgz
BuildArch: noarch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
C++ Algorithmic Differentiation (AD), see %{name}-devel, %{name}-doc.

# ---------------------------------------------------------------------------
%package devel
Summary: The %{name} C++ include files for Algorithmic Differentiation (AD)
Group: Development/Libraries
Provides: %{name} = %{version}-%{release}

%description devel
We refer to the step by step conversion from an algorithm that computes 
function values to an algorithm that computes derivative values as 
Algorithmic Differentiation (often referred to as Automatic Differentiation.) 
Given a C++ algorithm that computes function values, %{name} generates an 
algorithm that computes its derivative values. A brief introduction to 
Algorithmic Differentiation (AD) can be found at 
     http://en.wikipedia.org/wiki/Automatic_differentiation
See the package %{name}-doc for documentation of this version of %{name}. 

# ----------------------------------------------------------------------------
%package doc
Summary: Documentation for %{name}
Group: XML documentation for %{name}-devel 

%description doc
The %{name}-doc package installs the XML documentation for %{name}-devel in
     %{_docdir}/%{name}-%{version}
Links to an HTML version of the %{name} documentation, for the most recent 
version of %{name}, can be found at the web site 
     http://www.coin-or.org/CppAD/
In addition, printable versions of the documentation,
as a single web page, in both HTML and XML, can be found at that web site.

# ----------------------------------------------------------------------------
%prep

%setup -q 
# Patches 2 and 3 should not be necessary when %%{version} > 20110101.2.
# 1. CppAD tests the distribution before installing it. The rpm system can
# test after a mock installation. Thus, we should test the installed copy  
# instead of the distribution copy of the include files. We must skip 
# speed/src/makefile.in because it builds a library (that is not installed).
# 2. Change the location where pkgconfig/cppad.pc is installed.
# 3. Remove the duplicate copy of the test results from the rpm output.
includedir=%{buildroot}%{_includedir}
for file in `find . -name 'makefile.in'`
do
     if ! (echo $file | grep 'speed/src/makefile.in' > /dev/null)
     then
          sed -i "$file" \
          -e "s|-I\$(top_srcdir)\([^/]\)|-I$includedir\1|" \
          -e 's|^pkgconfigdir *=.*|pkgconfigdir = $(datadir)/pkgconfig|' \
          -e '/^[\t ]*cat test.log *$/d'
     fi
done


# Remove setting of permissions in documentation destination directory.
sed -i.stamp makefile.in -e '/chmod -R [^ ]* $(doc_package)/d'
#

# change example/example.cpp to print out machine epsilon 
cat << EOF > example.sed
/^int main(void)/N
s|^int main(void)\n{|# include <cfloat>\\
int main(void)\\
{       // print machine epsilon\\
        std::cout << "DBL_EPSILON =" << DBL_EPSILON << std::endl;\\
\\
|
EOF
sed -i.stamp example/example.cpp -f example.sed

%build
%configure --with-Documentation 
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%check
# make test uses a shell script to run make test is a list of subdirectories
# and it it is unclear how to do: make %{?_smp_mflags} test
make test

%clean
# cleanup 
rm -rf %{buildroot}

%files devel
%defattr(-,root,root,-)
%doc COPYING ChangeLog AUTHORS uw_copy_040507.html
%{_includedir}/%{name}
%{_datadir}/pkgconfig/%{name}.pc

%files doc
%defattr(-,root,root,-)
%{_docdir}/%{name}-%{version}

# ----------------------------------------------------------------------------
%changelog
* Tue Jan 19 2011 Brad Bell <bradbell at seanet dot com> - 20110101.2-3
- Remove duplicate test results from build.log
- Improve comments before patching top level makefile.in.
- Fix rpmlint warnings about %%{_docdir}, %%{name}, and %%{version} in comments.
- Fix rpmlint warning by changing tabs to spaces.
- Fix rpmlint wrning by removing dot at end of Summary.
- Change RPM_BUILD_ROOT to _builddir.
- Improve comments (at top) about know rpmlint warnings.

* Tue Jan 18 2011 Brad Bell <bradbell at seanet dot com> - 20110101.2-2
- Fix rpmlint error, libdir-macro-in-noarch-package by moving the pkg-config 
- file cppad.pc from %%_libdir to %%_datadir.
- Improve the %%Summary and %%description entries.
- Fix some rpmlint spelling warnings including xml -> XML, html -> HTML. 
- Document (at top of spec file) reason for other warnings that are not fixed.

* Mon Jan 17 2011 Brad Bell <bradbell at seanet dot com> - 20110101.2-1 
- The fedora source 20110101.0 has the worng check sum, get new upstream source.
- Remove sed patches for problems that were fixed in upstream source.
- Change makefile.in so tests include from install (not distribute) directory
- (see comments above `find . -name 'makefile.in` above).

* Sat Jan 09 2011 Brad Bell <bradbell at seanet dot com> - 20110101.0-2
- Remove include/cppad_ipopt_nlp.hpp from distribution.
- Remove lib/libspeed.a from distribution.
- Add lib/pkgconfig/cppad.pc to files section (because it is installed).
- Use a single sed script file with comments to do all the makefile.in edits.
- Remove edits of makefile.am (not used so not reason to patch it).

* Sat Jan 08 2011 Brad Bell <bradbell at seanet dot com> - 20110101.0-1
- Use new major version for 2011.
- abs_top_builddir is missing from definitions in makefile.in 
- (should be fixed in future versions of cppad).
- The single command "make test" now builds and runs all the tests.

* Thu Jul 08 2010 Brad Bell <bradbell at seanet dot com> - 20100101.4-1
- Use new upstream source which has bug fix at revision
- https://projects.coin-or.org/CppAD/changeset/1698

* Wed Mar 31 2010 Brad Bell <bradbell at seanet dot com> - 20100101.2-1
- Use new upstream source with bug fixes at revision
- https://projects.coin-or.org/CppAD/changeset/1664
- and remove patch from Wed Feb 10 2010. 

* Wed Feb 10 2010 Brad Bell <bradbell at seanet dot com> - 20100101.0-2
- Patch sources for bug fix between 20100101.0 and 20100101.1.
- This should no longer be necessary once a new upstream source is loaded.

* Sat Jan 01 2010 Brad Bell <bradbell at seanet dot com> - 20100101.0-1
- Use new upstream source.
- Remove out of date comment about where this spec file is maintained.
- Remove patches that are no longer necessary in prep section.
- Change calling sequence for correctness of speed tests (we do not run speed
- tests, that requires a computer with no other processes running).

* Fri Jul 24 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 20090303.0-5
- Rebuilt for https://fedoraproject.org/wiki/Fedora_12_Mass_Rebuild

* Sat Jun 20 2009 Brad Bell <bradbell at seanet dot com> 20090303-4
- Patch cppad/local/fun_construct.hpp to give a more useful error message
- (so we can figure out why the Fedora 11 build is failing).

* Mon Jun 06 2009 Brad Bell <bradbell at seanet dot com> 20090303-3
- Patch file test_more/jacobian.cpp (required for versions below 20090606).
- Patch file cppad/local/default.hpp (required for versions below 20090606).
- Fix version (change 20080303 to 20090303) in previous two log entries.

* Mon Mar 30 2009 Brad Bell <bradbell at seanet dot com> 20090303-2
- Change tabs to spaces in spec file to avoid an rpmlint warning.
- The base package in previous release had no files, hence did not exist.
- Use Provides: in cppad-devel to indicate that it provides cppad.

* Sun Mar 29 2009 Brad Bell <bradbell at seanet dot com> 20090303-1
- Change to newer version of cppad.
- Create a base package that requres both devel and doc sub-packages

* Tue Feb 24 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 20080826.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_11_Mass_Rebuild

* Tue Oct 08 2008 Brad Bell <bradbell at seanet dot com> 20080826-1
- Change to newer version of cppad.
- Change download directory to standard coin-or location.
- Remove editing of speed/main.cpp (no longer necessary).
- Add retape argument to check programs in speed directory.

* Fri Apr 04 2008 Brad Bell <bradbell at seanet dot com> 20080403-3
- Patch speed/main.cpp work with newer version of gcc
- (speed/main.cpp had not been tested with new version of gcc.)

* Thu Apr 03 2008 Brad Bell <bradbell at seanet dot com> 20080403-2
- Upload new source with the command
- make new-sources FILES="cppad-20080403.gpl.tgz"

* Thu Apr 03 2008 Brad Bell <bradbell at seanet dot com> 20080403-1
- New upstream version

* Sat Jan 12 2008  Brad Bell <bradbell at seanet dot com> 20071229-6
- Remove speed estimation correctness test because we are not in control of 
- which other jobs are on the machine that is doing the rpmbuild.

* Fri Jan 11 2008  Brad Bell <bradbell at seanet dot com> 20071229-5
- Remove introduction/exp_apx/exp_apx from the set of tests 
- (which should have been done in 20071229-4). 
- From now on test building rpm locally before making tags.

* Thu Jan 10 2008  Brad Bell <bradbell at seanet dot com> 20071229-4
- Add code to print out DBL_EPSILON at the beginning of the example tests.
- Remove --with-Introduction (it only checks by hand calculations that are in 
- AD Introduction section of the documentation). 
- Remove extra --with-Documentation

* Wed Jan 09 2008  Brad Bell <bradbell at seanet dot com> 20071229-3
- I mistakenly tried to make tag 20071229-2 in devel before committing local 
- changes. It appears tag was partially created, but not sure it is correct.
- So I am bumping the version number. 

* Wed Jan 09 2008  Brad Bell <bradbell at seanet dot com> 20071229-2
- Cygwin's version of md5sum puts a <space><star> between the check sum
- and the file name. Fedora build tools expect two spaces, so the star has
- was changed to a space in the devel, F-7, and F-8 sources file.

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
- Move -rf RPM_BUILD_ROOT from prep entry to install entry.
- Use macros where possible.

* Sat Dec 08 2007 Brad Bell <bradbell at seanet dot com> 20071208-1
- Fix all but one rpmlint warning (see Notes at beginning of this file).

* Mon Dec 03 2007 Brad Bell <bradbell at seanet dot com> 20071203-1
- first version of cppad that included RPM spec file.
