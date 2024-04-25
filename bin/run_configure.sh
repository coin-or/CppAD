#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
set -e -u
if [ ! -e "bin/run_configure.sh" ]
then
   echo "bin/run_configure.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
with_clang=''
cpp_standard='c++17'
while [ "$#" != '0' ]
do
   if [ "$1" == '--help' ]
   then
      cat << EOF
usage: bin/run_configure.sh \\
   [--help] \\
   [--with_clang] \\
   [--c++yy]
EOF
      exit 0
   fi
   case "$1" in

      --with_clang)
      with_clang='--with_clang'
      ;;

      --c++*)
      cpp_standard=$(echo "$1" | sed -e 's|^--||')
      ;;

      *)
      echo "$1 is an invalid option, try bin/run_configure.sh --help"
      exit 1

   esac
   shift
done
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
# prefix
eval `grep '^prefix=' bin/get_optional.sh`
if [[ "$prefix" =~ ^[^/] ]]
then
   prefix="$(pwd)/$prefix"
fi
echo "prefix=$prefix"
# -----------------------------------------------------------------------------
#
# PKG_CONFIG_PATH
PKG_CONFIG_PATH="$prefix/lib64/pkgconfig:$prefix/lib/pkgconfig"
PKG_CONFIG_PATH="$prefix/share/pkgconfig:$PKG_CONFIG_PATH"
export PKG_CONFIG_PATH
#
# testvector
testvector='cppad'
#
# cppad_cxx_flags
cppad_cxx_flags="-std=$cpp_standard -Wall -pedantic-errors -Wshadow"
cppad_cxx_flags="$cppad_cxx_flags -Wfloat-conversion -Wconversion"
#
# eigen_prefix, scaado_prefix
cxx_standard_year=$(echo $cpp_standard | sed -e 's|c++||')
if [ "$cxx_standard_year" -lt 14 ]
then
   eigen_prefix=''
else
   eigen_prefix="EIGEN_DIR=$prefix"
fi
if [ "$cxx_standard_year" -lt 17 ]
then
   scaado_prefix=''
else
   scaado_prefix="SACADO_DIR=$prefix"
fi
#
# ---------------------------------------------------------------------------
if [ ! -e build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
# -----------------------------------------------------------------------------
../configure \
   --prefix=$prefix \
   $with_clang \
   --with-stdvector \
   MAX_NUM_THREADS=32 \
   CXX_FLAGS="'$cppad_cxx_flags'" \
   ADOLC_DIR=$prefix \
   BOOST_DIR=$prefix \
   EIGEN_DIR=$prefix \
   FADBAD_DIR=$prefix \
   SACADO_DIR=$prefix \
   IPOPT_DIR=$prefix \
   $eigen_prefix \
   $scaado_prefix \
   TAPE_ADDR_TYPE=size_t \
   TAPE_ID_TYPE=size_t
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
