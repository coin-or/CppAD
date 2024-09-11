# ---------------------------------------------------------------------------
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ---------------------------------------------------------------------------
#
# source bin/dev_settings.sh
# Sets the development tool variables listed below to settings for this system.
#
# spdx_license_id
# Each file, except those specified by no_copyright_list, should have a line
# that ends with the following text:
spdx_license_id='EPL-2.0 OR GPL-2.0-or-later'
#
# no_copyright_list
# These files do not have the spdx license id in them.
# If an entry below is a directory it specifies all the files in the directory.
# BEGIN_SORT_THIS_LINE_PLUS_2
no_copyright_list='
   .circleci/config.yml
   .coin-or/projDesc.xml
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
