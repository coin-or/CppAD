# ---------------------------------------------------------------------------
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
# ---------------------------------------------------------------------------
# source bin/dev_settings.sh
# Sets the development tool variables listed below to settings for this system.
# ---------------------------------------------------------------------------
#
# Directories
# If an file name below is a directory it specifies all the
# files in the directory.
#
# spdx_license_id
# Each file, except those specified by no_copyright_list, should have a line
# that ends with the following text:
spdx_license_id='EPL-2.0 OR GPL-2.0-or-later'
#
# package_name
package_name='cppad'
#
# index_page_name
# is the xrst index page_name for this projects documentaiton.
index_page_name='user_guide'
#
# version_file_list
# The possible patterns for a latest version number are:
#     yyyymmdd or yyyy.month.day
# whee yyyymmdd is an eight decimal digit representaiton of the date.
# yyyy is the year (as four decimal digits yyy), month is a number
# between 1 and 12, and the day is a number between 1 and 31
# The possible patterns for a release version number are:
#     yyyy0000.release or yyyy.0.release
# where release is a number between 0 and 99.
#
# The patterns above without release are used for the master and main branches
# and corresponds to the current year, month and day.
# The patterns above with release are used for stable/* branches.
#
# The first version file of the list below must have one copy of its
# version surrounded by single or double quotes. This determines the verison
# when the branch is not master or main. All occurances of the version, in the
# files listed below, with the following forms are updated by check_verison.sh:
#     $package_name-$version  or '$version' or "$version"
#
# We use tag for the version corresponding to the current stable release.
# This is (is not) the same as the current version on a stable branch
# (on the master or main branch). All occurances of the tag, in the files
# listed below, with the following forms are updated by new_release.sh:
#     archive/$tag.tar.gz
# In addition, all occurances of stable-yyyy and release-yyyy are updated.
version_file_list='
   CMakeLists.txt
   user_guide.xrst
'
# All the occurances of the version in the files above are checked to see
# that they agree.
#
#
# no_copyright_list
# These files and dirctories do not have the spdx license id in them.
# If an entry below is a directory it specifies all the files in the directory.
# BEGIN_SORT_THIS_LINE_PLUS_2
no_copyright_list='
   .circleci/config.yml
   .github/workflows
   .gitignore
   .readthedocs.yaml
   .travis.yml
   COPYING
   appveyor.yml
   authors
   batch_edit.sed
   bin/build.bat
   cmake/cppad_uninstall.cmake
   coin.png
   epl-2.0.txt
   uw_copy_040507.html
   xrst.toml
'
# END_SORT_THIS_LINE_MINUS_2
#
# invisible_and_tab_ok
# These files are not checked for invisible white space or tabs.
# If an entry below is a directory it specifies all the files in the directory.
invisible_and_tab_ok='
   batch_edit.sed
   coin.png
   uw_copy_040507.html
'
#
# check_git_commit
# These files may have automatic changes that should not be commited every time.
# Including them in this list gives the user the option to abort their changes.
check_git_commit='
'
