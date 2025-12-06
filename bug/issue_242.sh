#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2025 Perry de Valpine <
# SPDX-FileContributor: 2025 Bradley M. Bell
# -----------------------------------------------------------------------------
set -e -u
#
# bash function that echos and executes a command
echo_eval() {
   echo $*
   eval $*
}
# -----------------------------------------------------------------------------
#
# name
name=`echo $0 | sed -e 's|^bug/||' -e 's|\.sh$||'`
if [ "$0" != "bug/$name.sh" ]
then
   echo "usage: bug/$name.sh"
   exit 1
fi
#
# PKG_CONFIG_PATH, LD_LIBRARY_PATH
prefix="$(pwd)/build/prefix"
export PKG_CONFIG_PATH="$prefix/share/pkgconfig"
export LD_LIBRARY_PATH=""
for subdir in  'lib' 'lib64' 
do
   PKG_CONFIG_PATH+=":$prefix/$subdir/pkgconfig"
   LD_LIBRARY_PATH+=";$prefix/$subdir"
done
#
# eigen_flags
eigen_flags=$(pkg-config -cflags eigen3)
#
# ipopt_flags
ipopt_flags=$(pkg-config -cflags ipopt | sed -e 's|/coin-or | |' )
#
# link_flags
link_flags=$(pkg-config -libs cppad)
#
# extra_flags
extra_flags='-Wall -pedantic-errors -std=c++11 -Wshadow -Wconversion -g -O0'
#
# build/bug
if [ -e build/bug ]
then
   rm -r build/bug
fi
mkdir -p build/bug
cd build/bug
# 
# $name.cpp
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>

bool pow_dynamic_cases(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   const size_t nd = 1, nx = 1, ny = 1;
   size_t abort_op_index = 0;
   bool   record_compare = false;
   bool verbose = true;
   // -----------------------------
   // Case A: x = variable, y = dynamic; y == 0 during recording
   // -----------------------------
   {
      if(verbose) 
         std::cout<<"pow(x, y) with y dynamic and zero during recording\n";
      CPPAD_TESTVECTOR(AD<double>) ax(nx), ay(ny), adyn(nd);
      ax[0]  = 2.0;      // x during recording
      adyn[0]= 0.0;      // y during recording (dynamic zero)

      CppAD::Independent(ax, abort_op_index, record_compare, adyn);

      AD<double> x = ax[0];
      AD<double> y = adyn[0]; // dynamic param (zero)
      ay[0] = pow(x, y);      // variable ^ dynamic 0

      CppAD::ADFun<double> f(ax, ay);

      // Evaluate with new dynamic values
      CPPAD_TESTVECTOR(double) x_in(nx), y_out(ny), dyn(nd);

      // First eval: y = 3 (non-zero), x = 2 => 2^3 = 8
      x_in[0] = 2.0;
      dyn[0]  = 3.0;
      f.new_dynamic(dyn);
      y_out = f.Forward(0, x_in);
      ok &= NearEqual(y_out[0], 8.0, eps99, eps99);

      if(verbose) 
        std::cout<<"pow(2,3) with this tape should give 8, but gives "<<y_out[0]<<"\n";

      // Second eval: y = 2, x = 3 => 3^2
      x_in[0] = 3.0;
      dyn[0]  = 2.0;
      f.new_dynamic(dyn);
      y_out = f.Forward(0, x_in);
      ok &= NearEqual(y_out[0], std::pow(3.0, 2.0), eps99, eps99);
      if(verbose) 
        std::cout<<"pow(3,2) with this tape should give 9, but gives "<<y_out[0]<<"\n";
   }

   // -----------------------------
   // Case B: x = dynamic (zero at recording), y = variable
   // -----------------------------
   {
      if(verbose) 
         std::cout<<"pow(x, y) with x dynamic and zero during recording\n";

      CPPAD_TESTVECTOR(AD<double>) ax(nx), ay(ny), adyn(nd);
      ax[0]   = 3.0;     // y during recording
      adyn[0] = 0.0;     // x during recording (dynamic zero)

      CppAD::Independent(ax, abort_op_index, record_compare, adyn);

      AD<double> x = adyn[0]; // dynamic param (zero)
      AD<double> y = ax[0];   // variable
      ay[0] = pow(x, y);      // dynamic 0 ^ variable

      CppAD::ADFun<double> f(ax, ay);

      // Evaluate with new dynamic values
      CPPAD_TESTVECTOR(double) x_in(nx), y_out(ny), dyn(nd);

      // First eval: x = 2 (non-zero), y = 3 => 2^3 = 8
      x_in[0] = 3.0;  // y
      dyn[0]  = 2.0;  // x
      f.new_dynamic(dyn);
      y_out = f.Forward(0, x_in);
      ok &= NearEqual(y_out[0], 8.0, eps99, eps99);

      if(verbose) 
        std::cout<<"pow(2,3) with this tape should give 8, but gives "<<y_out[0]<<"\n";

      // Second eval: x = 3, y = 2 => 3^2
      x_in[0] = 2.0;  // y
      dyn[0]  = 3.0;  // x
      f.new_dynamic(dyn);
      y_out = f.Forward(0, x_in);
      ok &= NearEqual(y_out[0], std::pow(3.0, 2.0), eps99, eps99);
      if(verbose) 
        std::cout<<"pow(3,2) with this tape should give 9, but gives "<<y_out[0]<<"\n";

   }

   return ok;
}
int main(void)
{  bool ok = true;
   //
   ok &= pow_dynamic_cases();
   //
   if( ok )
      return 0;
   return 1;
}
EOF
#
# $name
cmd="
   g++ 
   $name.cpp 
   -I../../include 
   $extra_flags
   $eigen_flags 
   $ipopt_flags 
   -o $name
   $link_flags 
"
#
$cmd
#
echo "build/bug/$name"
if ! ./$name
then
   echo
   echo "build/bug/$name: Error"
   exit 1
fi
echo
# -----------------------------------------------------------------------------
echo "bug/$name.sh: OK"
exit 0
