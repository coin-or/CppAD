// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin jit_compile.cpp}

JIT Compiler Options: Example and Test
######################################

compile
*******
This example demonstrates setting the JIT
:ref:`create_dll_lib@options@compile` option.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end jit_compile.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

# include <cstddef>
# include <iostream>
# include <fstream>
# include <map>

// DLL_EXT
# ifdef _WIN32
# define DLL_EXT ".dll"
# else
# define DLL_EXT ".so"
# endif

# include <cppad/cppad.hpp>
bool compile(void)
{  bool ok = true;
   //
   using CppAD::AD;
   using CppAD::ADFun;
   using CppAD::Independent;
   using CppAD::NearEqual;
   //
   // compile
   std::string compile = "";
# if CPPAD_C_COMPILER_MSVC_FLAGS
   if( std::system( CPPAD_C_COMPILER_CMD "/HELP 1> nul 2> nul") == 0 )
      compile = CPPAD_C_COMPILER_CMD " /EHs /EHc /c /TC /O2";
# endif
# if CPPAD_C_COMPILER_GNU_FLAGS
   if( std::system( CPPAD_C_COMPILER_CMD " --version > temp") == 0 )
      compile = CPPAD_C_COMPILER_CMD " -c -fPIC -O2";
# endif
   //
   if( compile == "" )
   {  std::cout << ": cannot determine C compiler to use so skiping test: ";
      return ok;
   }
   // std::cout << "compile = " << compile << "\n";
   //
   // nx, ny
   size_t nx = 2, ny = 1;
   //
   // f(x) = x_0 + x_1
   CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
   ax[0] = 0.0;
   ax[1] = 1.0;
   Independent(ax);
   ay[0] = ax[0] + ax[1];
   ADFun<double> f(ax, ay);
   f.function_name_set("f");
   //
   // csrc_file
   // created in std::filesystem::current_path
   std::string c_type    = "double";
   std::string csrc_file = "compile.c";
   std::ofstream ofs;
   ofs.open(csrc_file , std::ofstream::out);
   f.to_csrc(ofs, c_type);
   ofs.close();
   //
   // dll_file
   // created in std::filesystem::current_path
   std::string dll_file = "jit_compile" DLL_EXT;
   CPPAD_TESTVECTOR( std::string) csrc_files(1);
   csrc_files[0] = csrc_file;
   std::map< std::string, std::string > options;
   if( compile != "" )
      options["compile"] = compile;
   std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cerr << "jit_compile: err_msg = " << err_msg << "\n";
      return false;
   }
   // dll_linker
   CppAD::link_dll_lib dll_linker(dll_file, err_msg);
   if( err_msg != "" )
   {  std::cerr << "jit_compile: err_msg = " << err_msg << "\n";
      return false;
   }
   //
   // f_ptr
   std::string function_name = "cppad_jit_f";
   void* void_ptr = dll_linker(function_name, err_msg);
   if( err_msg != "" )
   {  std::cerr << "jit_compile: err_msg = " << err_msg << "\n";
      return false;
   }
   //
   // jit_double
   using CppAD::jit_double;
   //
   // f_ptr
   jit_double f_ptr =
      reinterpret_cast<jit_double>(void_ptr);
   //
   // x, y, compare_change
   // y = f(x)
   size_t compare_change = 0;
   std::vector<double> x(nx), y(ny);
   x[0] = 0.3;
   x[1] = 0.5;
   f_ptr(nx, x.data(), ny, y.data(), &compare_change);
   //
   // ok
   ok &= compare_change == 0;
   //
   // ok
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   double check = x[0] + x[1];
   ok &= NearEqual(y[0], check, eps99, eps99);
   //
   return ok;
}
// END C++
