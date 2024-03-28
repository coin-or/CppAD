#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
set -e -u
if [ ! -e "bin/run_cmake.sh" ]
then
   echo "bin/run_cmake.sh: must be executed from its parent directory"
   exit 1
fi
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
# PKG_CONFIG_PATH
PKG_CONFIG_PATH="$prefix/lib64/pkgconfig:$prefix/lib/pkgconfig"
PKG_CONFIG_PATH="$prefix/share/pkgconfig:$PKG_CONFIG_PATH"
export PKG_CONFIG_PATH
# -----------------------------------------------------------------------------
verbose_make='no'
standard='c++17'
profile_speed='no'
callgrind='no'
clang='no'
static='no'
yes_adolc='yes'
yes_colpack='yes'
yes_eigen='yes'
yes_ipopt='yes'
yes_fadbad='yes'
yes_cppad_jit='yes'
yes_cppadcg='yes'
yes_sacado='yes'
yes_documentation='yes'
addr_t_type='int'
testvector='boost'
debug_which='debug_all'
while [ "$#" != 0 ]
do
   if [ "$1" == '--help' ]
   then
      cat << EOF
usage: bin/run_cmake.sh: \\
   [--help] \\
   [--verbose_make] \\
   [--c++11] \\
   [--profile_speed] \\
   [--callgrind] \\
   [--clang ] \\
   [--static] \\
   [--no_adolc] \\
   [--no_colpack] \\
   [--no_eigen] \\
   [--no_ipopt] \\
   [--no_fadbad] \\
   [--no_cppad_jit] \\
   [--no_cppadcg] \\
   [--no_sacado] \\
   [--no_optional] \\
   [--no_documentation] \\
   [--addr_t_<type>] \\
   [--<package>_vector] \\
   [--debug_<which>]
The --help option just prints this message and exits.
The value <package> above must be one of: cppad, boost, eigen, or std.
The value <which> must be one of: odd, even, all, none.
The value <type> must be one of: size_t, int, unsigned_int

EOF
      exit 0
   fi
   case "$1" in

      --verbose_make)
      verbose_make='yes'
      ;;

      --c++11)
      standard='c++11'
      ;;

      --profile_speed)
      profile_speed='yes'
      ;;

      --callgrind)
      callgrind='yes'
      ;;

      --clang)
      clang='yes'
      ;;

      --static)
      static='yes'
      ;;

      --no_adolc)
      yes_adolc='no'
      ;;

      --no_colpack)
      yes_colpack='no'
      ;;

      --no_eigen)
      yes_eigen='no'
      ;;

      --no_ipopt)
      yes_ipopt='no'
      ;;

      --no_cppadcg)
      yes_cppadcg='no'
      ;;

      --no_cppad_jit)
      yes_cppad_jit='no'
      ;;

      --no_fadbad)
      yes_fadbad='no'
      ;;

      --no_sacado)
      yes_sacado='no'
      ;;

      --no_optional)
      yes_adolc='no'
      yes_colpack='no'
      yes_eigen='no'
      yes_ipopt='no'
      yes_cppadcg='no'
      yes_fadbad='no'
      yes_sacado='no'
      testvector='cppad'
      ;;

      --no_documentation)
      yes_documentation='no'
      ;;

      # ----------------------------------------------------------------------
      --addr_t_size_t)
      addr_t_type='size_t'
      ;;

      --addr_t_int)
      addr_t_type='int'
      ;;

      --addr_t_unsigned_int)
      addr_t_type='unsigned int'
      ;;

      # ----------------------------------------------------------------------
      --cppad_vector)
      testvector='cppad'
      ;;

      --boost_vector)
      testvector='boost'
      ;;

      --eigen_vector)
      testvector='eigen'
      ;;

      --std_vector)
      testvector='std'
      ;;

      # ----------------------------------------------------------------------
      --debug_odd)
      debug_which='debug_odd'
      ;;

      --debug_even)
      debug_which='debug_even'
      ;;

      --debug_all)
      debug_which='debug_all'
      ;;

      --debug_none)
      debug_which='debug_none'
      ;;

      # ----------------------------------------------------------------------
      *)
      echo "$1 is an invalid option, try bin/run_cmake.sh --help"
      exit 1
   esac
   shift
done
if [ "$standard" == 'c++11' ]
then
   # Scacdo and eigen cannot be used with c++11
   if [ "$yes_eigen" == 'yes' ]
   then
      echo 'Cannot use eigen with c++11'
      exit 1
   fi
   if [ "$yes_sacado" == 'yes' ]
   then
      echo 'Cannot use sacado with c++11'
      exit 1
   fi
fi
# ---------------------------------------------------------------------------
if [ ! -e build ]
then
   echo_eval mkdir build
fi
echo_eval cd build
if [ -e CMakeCache.txt ]
then
   echo_eval rm CMakeCache.txt
fi
if [ -e CMakeFiles ]
then
   echo_eval rm -r CMakeFiles
fi
# -----------------------------------------------------------------------------
# clean all variables in cmake cache
cmake_args='-U .+ -D cmake_defined_ok=FALSE'
# -----------------------------------------------------------------------------
# Generator
if [ "$verbose_make" == 'yes' ]
then
   cmake_args="$cmake_args  -G 'Unix Makefiles' -D CMAKE_VERBOSE_MAKEFILE=YES"
else
   cmake_args="$cmake_args  -G Ninja"
fi
# -----------------------------------------------------------------------------
# cppad_prefix
cmake_args="$cmake_args  -D cppad_prefix=$prefix"
#
# cmake_install_includedirs
if [ -d '/usr/include' ]
then
   cmake_args="$cmake_args -D cmake_install_includedirs=include"
fi
#
# cmake_install_datadir
if [ -d '/usr/share' ]
then
   cmake_args="$cmake_args -D cmake_install_datadir=share"
fi
#
# cmake_install_docdir
if [ -d '/usr/share' ] && [ "$yes_documentation" == 'yes' ]
then
   cmake_args="$cmake_args -D cmake_install_docdir=share/doc"
fi
#
# cmake_install_libdirs
if [ -d '/usr/lib64' ]
then
   cmake_args="$cmake_args -D cmake_install_libdirs='lib64;lib'"
elif [ -d '/usr/lib' ]
then
   cmake_args="$cmake_args -D cmake_install_libdirs='lib;lib64'"
fi
#
# {package}_prefix
prefix_list=''
include_list=''
if [ "$yes_cppadcg" == 'yes' ]
then
   if [ ! -e "$prefix/include/cppad/cg/cg.hpp" ]
   then
      echo "Cannot find $prefix/include/cppad/cg/cg.hpp"
      exit 1
   fi
   include_list="$include_list cppadcg"
fi
if [ "$yes_fadbad" == 'yes' ]
then
   if [ ! -e "$prefix/include/FADBAD++/badiff.h" ]
   then
      echo "Cannot find $prefix/include/FADBAD++/badiff.h"
      exit 1
   fi
   prefix_list="$prefix_list fadbad"
fi
if [ "$yes_adolc" == 'yes' ]
then
   if [ ! -d "$prefix/include/adolc" ]
   then
      echo "Cannot file $prefix/include/adolc"
      exit 1
   fi
   include_list="$include_list adolc"
fi
if [ "$yes_colpack" == 'yes' ]
then
   if [ ! -e "$prefix/include/ColPack" ]
   then
      echo "Cannot find $prefix/include/ColPack"
      exit 1
   fi
   prefix_list="$prefix_list colpack"
fi
if [ "$yes_eigen" == 'yes' ]
then
   if [ ! -e "$prefix/include/Eigen" ]
   then
      echo "Cannot find $prefix/include/Eigen"
      exit 1
   fi
   include_list="$include_list eigen"
fi
if [ "$yes_ipopt" == 'yes' ]
then
   if [ ! -e "$prefix/include/coin-or/IpNLP.hpp" ]
   then
      echo "Cannot find $prefix/include/coin-or/IpoptConfig.hpp"
      exit 1
   fi
   include_list="$include_list ipopt"
fi
if [ "$yes_sacado" == 'yes' ]
then
   if [ ! -e "$prefix/include/Sacado_config.h" ]
   then
      echo "Cannot find $prefix/include/Sacado_config.h"
      exit
   fi
   prefix_list="$prefix_list sacado"
fi
for package in $include_list
do
   cmake_args="$cmake_args -D include_${package}=true"
done
for package in $prefix_list
do
   cmake_args="$cmake_args  -D ${package}_prefix=$prefix"
done
#
# cppad_cxx_flags
cppad_cxx_flags="-Wall -pedantic-errors -std=$standard -Wshadow"
cppad_cxx_flags="$cppad_cxx_flags -Wfloat-conversion -Wconversion"
if [ "$debug_which" == 'debug_all' ]
then
   # CMAKE_CXX_FLAGS_DEBUG include -g so do not need it here
   cppad_cxx_flags="$cppad_cxx_flags -O0"
elif [ "$callgrind" == 'yes' ]
then
   # This is a quote from the Callgrind manual:
   # 'As with Cachegrind, you probably want to compile with debugging info
   # (the -g option) and with optimization turned on.'
   cmake_args="$cmake_args -g"
fi
#
# cppad_cxx_flags
# clang++ 14.05 is warnings on bitwise operations with logical operands.
# These are used for speed, but maybe they do not help much ?
if [ "$clang" == 'yes' ]
then
   cppad_cxx_flags="$cppad_cxx_flags -Wno-bitwise-instead-of-logical"
fi
#
# cmake_args
cmake_args="$cmake_args -D cppad_cxx_flags='$cppad_cxx_flags'"
#
# clang
if [ "$clang" == 'yes' ]
then
   cmake_args="$cmake_args -D CMAKE_C_COMPILER=clang"
   cmake_args="$cmake_args -D CMAKE_CXX_COMPILER=clang++"
fi
#
# static
if [ "$static" == 'yes' ]
then
   cmake_args="$cmake_args -D cppad_static_lib=true"
fi
#
# profile
if [ "$profile_speed" == 'yes' ]
then
   cmake_args="$cmake_args -D cppad_profile_flag=-pg"
fi
#
# debug_and_release
if [ "$debug_which" == 'debug_none' ] || [ "$debug_which" == 'debug_all' ]
then
   cmake_args="$cmake_args -D cppad_debug_and_release=false"
fi
#
# simple options
cmake_args="$cmake_args -D cppad_testvector=$testvector"
cmake_args="$cmake_args -D cppad_debug_which=$debug_which"
cmake_args="$cmake_args -D cppad_max_num_threads=48"
cmake_args="$cmake_args -D cppad_tape_id_type='$addr_t_type'"
cmake_args="$cmake_args -D cppad_tape_addr_type='$addr_t_type'"
#
echo_eval cmake $cmake_args ..
#
# ----------------------------------------------------------------------------
echo "$0: OK"
exit 0
