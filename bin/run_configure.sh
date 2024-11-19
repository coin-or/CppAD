#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
set -e -u
echo $0 $*
# ----------------------------------------------------------------------------
if [ ! -e "bin/run_configure.sh" ]
then
   echo "bin/run_configure.sh: must be executed from its parent directory"
   exit 1
fi
# -----------------------------------------------------------------------------
with_clang=''
with_verbose_make=''
cpp_standard='c++17'
with_vector=''
while [ "$#" != '0' ]
do
   if [ "$1" == '--help' ]
   then
      cat << EOF
usage: bin/run_configure.sh \\
   [--help] \\
   [--clang] \\
   [--verbose_make] \\
   [--c++<yy> ] \\
   [--<package>_vector]
The value yy is two decimal digits specifying the C++ standard year.
The value <package> must be one of: cppad, boost, eigen, std.

EOF
      exit 0
   fi
   case "$1" in

      --clang)
      with_clang='--with-clang'
      ;;

      --verbose_make)
      with_verbose_make='--with-verbose-make'
      ;;

      --c++*)
      cpp_standard=$(echo "$1" | sed -e 's|^--||')
      ;;

      --cppad_vector)
      with_vector=''
      ;;

      --boost_vector)
      with_vector='--with-boostvector'
      ;;

      --eigen_vector)
      with_vector='--with-eigenvector'
      ;;

      --std_vector)
      with_vector='--with-stdvector'
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
# cppad_cxx_flags
cppad_cxx_flags="-std=$cpp_standard -Wall -pedantic-errors -Wshadow"
cppad_cxx_flags+=" -Wfloat-conversion -Wconversion"
if [ "$(uname)" == 'Darwin' ]
then
   if which brew > /dev/null
   then
      cppad_cxx_flags+=" -I $(brew --prefix)/include"
   fi
fi
# 2DO: clang++ 14.05 is generating a lot of warnings (we should fix these)
#
# scaado_prefix
cxx_standard_year=$(echo $cpp_standard | sed -e 's|c++||')
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
   $with_verbose_make \
   $with_vector \
   MAX_NUM_THREADS=32 \
   CXX_FLAGS="'$cppad_cxx_flags'" \
   ADOLC_DIR=$prefix \
   FADBAD_DIR=$prefix \
   IPOPT_DIR=$prefix \
   $scaado_prefix \
   TAPE_ADDR_TYPE=size_t \
   TAPE_ID_TYPE=size_t
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
