// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin jit_get_started.cpp}

JIT Computation of Derivatives: Example and Test
################################################

Purpose
*******
This is a simple example using CppAD for Just In Time (JIT)
compilation, linking, and running of C source code that computes derivatives.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end jit_get_started.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

# include <cstddef>
# include <iostream>
# include <fstream>
# include <map>

# include <cppad/configure.hpp>
# if CPPAD_USE_CPLUSPLUS_2017
# include <filesystem>
# endif

// DLL_EXT
# ifdef _WIN32
# define DLL_EXT ".dll"
# else
# define DLL_EXT ".so"
# endif

# include <cppad/cppad.hpp>
bool get_started(void)
{  bool ok = true;
   //
   // p
# if CPPAD_USE_CPLUSPLUS_2017
   std::filesystem::path p;
# endif
   //
   using CppAD::AD;
   using CppAD::ADFun;
   using CppAD::Independent;
   using CppAD::NearEqual;
   //
   // f
   // f_0 (x_0, x_1) =  x_0 * cos(x_1)
   // f_1 (x_0, x_1) =  x_0 * sin(x_1)
   size_t n = 2;
   size_t m = 2;
   CPPAD_TESTVECTOR( AD<double> ) ax(n), ay(m);
   for(size_t j = 0; j < n; ++j)
      ax[j] = AD<double>( j + 1);
   Independent(ax);
   ay[0] = ax[0] * cos( ax[1] );
   ay[1] = ax[0] * sin( ax[1] );
   ADFun<double> f(ax, ay);
   ADFun< AD<double> , double > af = f.base2ad();
   //
   // g(x) = f'(x)
   // g(x) = [ cos(x_1) , - x_0 * sin(x_1) ]
   //        [ sin(x_1) ,   x_0 * cos(x_1) ]
   Independent(ax);
   CPPAD_TESTVECTOR( AD<double> ) az(m * n);
   az = af.Jacobian(ax);
   ADFun<double> g(ax, az);
   g.function_name_set("g");
   //
   // csrc_file
   // created in std::filesystem::current_path()
   std::string c_type    =  "double";
   std::string csrc_file = "get_started.c";
   std::ofstream ofs;
   ofs.open(csrc_file , std::ofstream::out);
   g.to_csrc(ofs, c_type);
   ofs.close();
   //
   // p, ok
# if CPPAD_USE_CPLUSPLUS_2017
   p   = std::filesystem::current_path();
   p  /= csrc_file;
   ok &= std::filesystem::exists(p);
# endif
   //
   // dll_file
   // created in std::filesystem::current_path()
   std::string dll_file = "jit_get_started" DLL_EXT;
   CPPAD_TESTVECTOR( std::string) csrc_files(1);
   csrc_files[0] = csrc_file;
   std::map< std::string, std::string > options;
   std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cerr << "jit_get_started: err_msg = " << err_msg << "\n";
      return false;
   }
   //
   // p, ok
# if CPPAD_USE_CPLUSPLUS_2017
   p   = std::filesystem::current_path();
   p  /= dll_file;
   ok &= std::filesystem::exists(p);
# endif
   //
   // dll_linker
   CppAD::link_dll_lib dll_linker(dll_file, err_msg);
   if( err_msg != "" )
   {  std::cerr << "jit_get_started: err_msg = " << err_msg << "\n";
      return false;
   }
   //
   // g_ptr
   std::string function_name = "cppad_jit_g";
   void* void_ptr = dll_linker(function_name, err_msg);
   if( err_msg != "" )
   {  std::cerr << "jit_get_started: err_msg = " << err_msg << "\n";
      return false;
   }
   //
   // jit_double
   using CppAD::jit_double;
   //
   // g_ptr
   jit_double g_ptr =
      reinterpret_cast<jit_double>(void_ptr);
   //
   // x, z, compare_change
   // z = g(x)
   size_t nx = n, nz = m * n, compare_change = 0;
   std::vector<double> x(nx), z(nz);
   x[0] = 0.3;
   x[1] = 0.5;
   g_ptr(nx, x.data(), nz, z.data(), &compare_change);
   //
   // ok
   ok &= compare_change == 0;
   //
   // eps99
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   //
   // ok
   // df_0 / dx_0 is stored in z[0 * n + 0]
   double value = z[ 0 * n + 0];
   double check = std::cos(x[1]);
   ok &= NearEqual(value, check, eps99, eps99);
   //
   // ok
   // df_0 / dx_1 is stored in z[0 * n + 1]
   value = z[ 0 * n + 1];
   check = - x[0] * std::sin(x[1]);
   ok &= NearEqual(value, check, eps99, eps99);
   //
   // ok
   // df_1 / dx_0 is stored in z[1 * n + 0]
   value = z[ 1 * n + 0];
   check = std::sin(x[1]);
   ok &= NearEqual(value, check, eps99, eps99);
   //
   // ok
   // df_1 / dx_1 is stored in z[1 * n + 1]
   value = z[ 1 * n + 1];
   check = x[0] * std::cos(x[1]);
   ok &= NearEqual(value, check, eps99, eps99);
   //
   return ok;
}
// END C++
