# ----------------------------------------------------------------------------
# None of the lists below can have white space or a dollar sign in an entry.
#
# The person of company that owns the copyright for this package
# (if empty then no copyright for this package).
copyright_owner='Bradley M. Bell'
#
# List of files, besides CMakeLists.txt, that have have a copy of the
# version number (can be empty).
version_files='
   cppad.xrst
   configure.ac
'
#
# List of special files, for this repository, that the devel tools ignore.
# The files /.gitignore, /batch_edit.sed, /bin/devel.sh should be in this list.
# Files that are created by a program, and checked into the repository,
# should also be in this list. Names that end with / are directories to ignore.
# Names that begin with a / are relative to top source directroy.
# All other names are relavie paths somewhere below the top source directory.
# BEGIN_SORT_THIS_LINE_PLUS_2
ignore_files='
   /.circleci/
   /.coin-or/projDesc.xml
   /.github/workflows/
   /.gitignore
   /COPYING
   /ar-lib
   /authors
   /batch_edit.sed
   /bin/build.bat
   /bin/devel.sh
   /bin/get_highlight.sh
   /bin/get_omhelp.sh
   /cmake/cppad_uninstall.cmake
   /coin.png
   /compile
   /config.guess
   /config.sub
   /configure
   /configure
   /depcomp
   /epl-2.0.txt
   /install-sh
   /missing
   /sphinx/preamble.rst
   /uw_copy_040507.html
   /xrst.toml
   makefile.in
'
# END_SORT_THIS_LINE_MINUS_2
# ----------------------------------------------------------------------------
echo "copyright_owner=$copyright_owner"
echo "image_link=$version_files"
echo "ignore_files=$ignore_files"
echo "version_files=$version_files"
# ----------------------------------------------------------------------------
