#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
if [ "$0" != "bin/package.sh" ]
then
   echo "bin/package.sh: must be executed from its parent directory"
   exit 1
fi
echo_eval() {
     echo $*
     eval $*
}
# -----------------------------------------------------------------------------
# index_page_name
index_page_name=$(\
   sed -n -e '/^ *--index_page_name*/p' .readthedocs.yaml | \
   sed -e 's|^ *--index_page_name *||' \
)
#
# version
version=`sed -n -e '/^SET( *cppad_version *"[0-9.]*"/p' CMakeLists.txt | \
   sed -e 's|.*"\([^"]*\)".*|\1|'`
#
# build
if [ ! -e 'build' ]
then
   echo_eval mkdir build
fi
#
# build/cppad-version
if ls build/cppad-* >& /dev/null
then
   echo_eval rm -r build/cppad-*
fi
mkdir build/cppad-$version
git ls-files -z | xargs -0 tar -czf build/cppad-$version/tar.tgz
cd build/cppad-$version
tar -xzf tar.tgz
rm tar.tgz
#
#  build/cppad-version/build/html
bin/run_xrst.sh -dev
#
# build/cppad-$version.tgz
cd ..
echo_eval tar -czf cppad-$version.tgz cppad-$version
echo_eval rm -r cppad-$version
#
echo 'package.sh: OK'
exit 0
