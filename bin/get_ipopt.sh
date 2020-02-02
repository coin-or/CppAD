#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
# $begin get_ipopt.sh$$ $newlinech #$$
# $spell
#   tgz
#   Ipopt
#   CppAD
#   Blas
#   Lapack
# $$
#
# $section Download and Install Ipopt in Build Directory$$
#
# $head Syntax$$
# $code bin/get_ipopt.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install
# $href%http://www.coin-or.org/projects/Ipopt.xml%Ipopt%$$ in the
# CppAD $code build$$ directory.
#
# $head Requirements$$
# This It is assumed that a copy of the Blas and Lapack is installed on
# the system.
#
# $head Distribution Directory$$
# This command must be executed in the
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head Source Directory$$
# The Ipopt source code is downloaded and compiled in the sub-directory
# $codei%build/external/Ipopt-%version%$$ below the distribution directory.
#
# $head Prefix$$
# The $cref/prefix/get_optional.sh/prefix/$$
# in the file $code bin/get_optional$$ is used this install.
#
# $head Version$$
# This will install the following version of Ipopt
# $srccode%sh%
version='3.12.9'
# %$$
#
# $head Configuration$$
# If the file
# $codei%
#   build/external/ipopt-%version%.configured
# %$$
# exists, the configuration will be skipped.
# Delete this file if you want to re-run the configuration.
#
# $end
# -----------------------------------------------------------------------------
package='ipopt'
if [ $0 != "bin/get_$package.sh" ]
then
    echo "bin/get_$package.sh: must be executed from its parent directory"
    exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
web_page='http://www.coin-or.org/download/source/Ipopt'
cppad_dir=`pwd`
# -----------------------------------------------------------------------------
# prefix
eval `grep '^prefix=' bin/get_optional.sh`
if [[ "$prefix" =~ ^[^/] ]]
then
    prefix="$cppad_dir/$prefix"
fi
echo "prefix=$prefix"
# -----------------------------------------------------------------------------
configured_flag="build/external/$package-${version}.configured"
echo "Executing get_$package.sh"
if [ -e "$configured_flag" ]
then
    echo "Skipping configuration because $configured_flag exits"
    echo_eval cd build/external/Ipopt-$version/build
    echo_eval make install
    echo "get_$package.sh: OK"
    exit 0
fi
# -----------------------------------------------------------------------------
if [ ! -d build/external ]
then
    echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "Ipopt-$version.tgz" ]
then
    echo_eval wget --no-check-certificate "$web_page/Ipopt-$version.tgz"
fi
if [ ! -e "Ipopt-$version" ]
then
    echo_eval tar -xzf Ipopt-$version.tgz
fi
echo_eval cd "Ipopt-$version"
# -----------------------------------------------------------------------------
for package in Metis Mumps
do
    if [ ! -e "ThirdParty/$package/get.$package.done" ]
    then
        echo_eval cd ThirdParty/$package
        echo_eval "./get.$package"
        echo_eval touch "get.$package.done"
        echo_eval cd ../..
    fi
done
# -----------------------------------------------------------------------------
if [ -e /usr/lib64 ]
then
    libdir='lib64'
else
    libdir='lib'
fi
if [ ! -e build ]
then
    mkdir build
fi
cd build
export PKG_CONFIG_PATH="$prefix/$libdir/pkgconfig"
echo_eval ../configure \
    --enable-debug \
    --prefix="$prefix" \
    --libdir="$prefix/$libdir" \
    coin_skip_warn_cxxflags='yes' | tee configure.log
for package in BLAS LAPACK Mumps Metis
do
    if ! grep " $package[.]* *yes:" configure.log > /dev/null
    then
        echo "Ipopt cannot find $package"
        exit 1
    fi
done
echo_eval make install
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$configured_flag
echo "get_$package.sh: OK"
