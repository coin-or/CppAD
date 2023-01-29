// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin jit_atomic.cpp}

Atomic Callbacks in JIT Function: Example and Test
##################################################

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end jit_atomic.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cppad/utility/link_dll_lib.hpp>

# if CPPAD_USE_CPLUSPLUS_2017
# include <filesystem>
# endif

//  DLL_EXT
# ifndef _WIN32
# define DLL_EXT         ".so"
# else
# define DLL_EXT         ".dll"
# endif

// ----------------------------------------------------------------------------
namespace { // BEGIN_EMPTY_NAMESPACE
//
// write_c_file
std::string write_c_file(size_t index, const std::string& csrc)
{  //
   std::string file_name = "atomic_" + CppAD::to_string(index) + ".c";
   //
   // write file_name
   std::ofstream os;
   os.open(file_name, std::ios::out);
   os << csrc;
   os.close();
   //
   return file_name;
}
//
// atomic_fun
class atomic_fun : public CppAD::atomic_four<double> {
private:
   const std::string name_;
public:
   atomic_fun(const std::string& name) :
   CppAD::atomic_four<double>(name),
   name_(name)
   {}
private:
   bool for_type(
      size_t                                     call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
   {  type_y[0] = type_x[0];
      return true;
   }
   // forward double
   bool forward(
      size_t                              call_id      ,
      const CppAD::vector<bool>&          select_y     ,
      size_t                              order_low    ,
      size_t                              order_up     ,
      const CppAD::vector<double>&        taylor_x     ,
      CppAD::vector<double>&              taylor_y     ) override
   {  if( order_up != 0 )
         return false;;
      taylor_y[0] = 1.0 / taylor_x[0];
      return true;
   }
public:
   // forward_zero
   std::string forward_zero(void)
   {  std::string csrc =
         "# include <stddef.h>\n"
         "int cppad_atomic_" + name_ + "(\n";
      csrc +=R"_(
   size_t        call_id,
   size_t        nx,
   const double* x,
   size_t        ny,
   double*       y,
   size_t*       compare_change)
{  if( nx != 1 ) return 1;
   if( ny != 1 ) return 2;
   y[0] = 1.0 / x[0];
   return 0;
}
)_";
      return csrc;
   }
};
} // END_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
bool atomic(void)
{  // ok
   bool ok = true;
   //
   // AD
   using CppAD::AD;
   //
   // function_name
   std::string function_name = "reciprocal";
   //
   // reciprocal
   atomic_fun reciprocal(function_name);
   //
   // nx, ax
   size_t nx = 2;
   CPPAD_TESTVECTOR( AD<double> ) ax(nx);
   double x0 = 0.5, x1 = 4.0;
   ax[0] = x0;
   ax[1] = x1;
   CppAD::Independent(ax);
   //
   // ny, ay
   size_t ny = nx;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);
   CPPAD_TESTVECTOR( AD<double> ) atom_x(1), atom_y(1);
   for(size_t j = 0; j < nx; ++j)
   {  atom_x[0] = ax[j];
      reciprocal(atom_x, atom_y);
      ay[j] = atom_y[0];
   }
   //
   // function_name
   function_name = "use_reciprocal";
   //
   // f
   CppAD::ADFun<double> f(ax, ay);
   f.function_name_set(function_name);
   //
   // dll_file
   std::string dll_file = "atomic" DLL_EXT;
   //
   // csrc_files
   CppAD::vector<std::string> csrc_files(2);
   csrc_files[0]      = write_c_file(0, reciprocal.forward_zero() );
   std::string c_type = "double";
   std::stringstream ss;
   f.to_csrc(ss, c_type);
   csrc_files[1] = write_c_file(1, ss.str() );
   //
   // create_dll_lib
   std::map< std::string, std::string > options;
   std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cout << "jit_atomic: " << err_msg << "\n";
      ok = false;
      return ok;
   }
   //
   // dll_linker
   CppAD::link_dll_lib dll_linker(dll_file, err_msg);
   //
   // jit_double
   using CppAD::jit_double;
   //
   // jit_function
   jit_double jit_function = nullptr;
   if( err_msg != "" )
   {  std::cout << "jit_atomic: " << err_msg << "\n";
      ok = false;
   }
   else
   {  // jit_function
      std::string complete_name = "cppad_jit_" + function_name;
      jit_function = reinterpret_cast<jit_double>(
            dll_linker(complete_name, err_msg)
      );
      if( err_msg != "" )
      {  std::cout << "jit_atomic: " << err_msg << "\n";
         ok = false;
      }
   }
   if( ok )
   {  //
      // ok
      // no change
      CppAD::vector<double> x(nx), y(ny);
      x[0] = x0;
      x[1] = x1;
      for(size_t i = 0; i < ny; ++i)
         y[i] = std::numeric_limits<double>::quiet_NaN();
      size_t compare_change = 0;
      int flag = jit_function(
         nx, x.data(), ny, y.data(), &compare_change
      );
      ok &= flag == 0;
      ok &= compare_change == 0;
      for(size_t i = 0; i < ny; ++i)
         ok &= y[i] == 1.0 / x[i];
   }
   return ok;
}
// END C++
