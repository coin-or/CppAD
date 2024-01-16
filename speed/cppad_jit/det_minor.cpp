// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cppad_jit_det_minor.cpp}

cppad_jit Speed: Gradient of Determinant by Minor Expansion
###########################################################

Specifications
**************
See :ref:`link_det_minor-name` .

Implementation
**************
{xrst_spell_off}
{xrst_code cpp} */
# include <map>
# include <cppad/cppad.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>
extern std::map<std::string, bool> global_option;

# ifdef _WIN32
# define DLL_EXT ".dll"
# else
# define DLL_EXT ".so"
# endif

# if ! (CPPAD_C_COMPILER_GNU_FLAGS || CPPAD_C_COMPILER_MSVC_FLAGS )
bool link_det_minor(
   const std::string&         job      ,
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &matrix   ,
   CppAD::vector<double>     &gradient )
{  return false; }
# else
namespace {
   //
   // using
   using std::string;
   //
   // typedefs
   typedef CppAD::AD<double>        a_double;
   typedef CppAD::vector<double>    d_vector;
   typedef CppAD::vector<a_double>  ad_vector;
   //
   // get_function_ptr
   CppAD::jit_double get_function_ptr(
      CppAD::link_dll_lib* dll_linker )
   {  std::string function_name = "cppad_jit_gradient_det";
      string err_msg;
      void* void_ptr = (*dll_linker)(function_name, err_msg);
      if( err_msg != "" )
      {  std::cerr << "link_det_minor: err_msg = " << err_msg << "\n";
         return nullptr;
      }
      CppAD::jit_double function_ptr =
            reinterpret_cast<CppAD::jit_double>(void_ptr);
      return function_ptr;
   }
   //
   // setup
   CppAD::link_dll_lib* setup(size_t size)
   {  // optimization options
      string optimize_options =
         "no_conditional_skip no_compare_op no_print_for_op";
      //
      // adet
      // object for computing determinant
      CppAD::det_by_minor<a_double>   adet(size);
      //
      // nx
      // number of independent variables
      size_t nx = size * size;
      //
      // matrix
      CppAD::vector<double> matrix(nx);
      CppAD::uniform_01(nx, matrix);
      //
      // aA
      ad_vector   aA(nx);
      for(size_t j = 0; j < nx; ++j)
         aA[j] = matrix[j];
      //
      // Independent
      // declare independent variables for function computation
      bool record_compare   = false;
      size_t abort_op_index = 0;
      CppAD::Independent(aA, abort_op_index, record_compare);
      //
      // adet_A
      // AD computation of the determinant
      ad_vector adet_A(1);
      adet_A[0] = adet(aA);
      //
      // f
      // f( matrix ) = det(matrix)
      CppAD::ADFun<double>  f;
      f.Dependent(aA, adet_A);
      if( global_option["optimize"] )
         f.optimize(optimize_options);
      //
      // af
      CppAD::ADFun<a_double, double> af;
      af = f.base2ad();
      //
      // Independent
      // declare independent variables for gradient computation
      CppAD::Independent(aA, abort_op_index, record_compare);
      //
      // aw
      // vectors of reverse mode weights
      CppAD::vector<a_double> aw(1);
      aw[0] = a_double(1.0);
      //
      // agradient
      ad_vector agradient(nx);
      af.Forward(0, aA);
      agradient = af.Reverse(1, aw);
      //
      // g
      // function objects for g : A -> det'( detA  )
      CppAD::ADFun<double> g;
      g.Dependent(aA, agradient);
      if( global_option["optimize"] )
         g.optimize(optimize_options);
      g.function_name_set("gradient_det");
      //
      // csrc_file
      string type      = "double";
      string csrc_file = "gradient_det.c";
      std::ofstream ofs;
      ofs.open(csrc_file, std::ofstream::out);
      g.to_csrc(ofs, type);
      ofs.close();
      //
      // dll_file
      string dll_file = "gradient_det" DLL_EXT;
      CppAD::vector< string > csrc_files(1);
      csrc_files[0] = csrc_file;
      std::map< string, string > dll_options;
# if CPPAD_C_COMPILER_MSVC_FLAGS
      dll_options["compile"] = CPPAD_C_COMPILER_CMD " /EHs /EHc /c /TC /O2";
# endif
# if CPPAD_C_COMPILER_GNU_FLAGS
      dll_options["compile"] = "gcc -c -fPIC -O2";
# endif
      string err_msg =
         CppAD::create_dll_lib(dll_file, csrc_files, dll_options);
      if( err_msg != "" )
      {  std::cerr << "link_det_minor: err_msg = " << err_msg << "\n";
         return nullptr;
      }
      //
      // dll_linker_ptr
      CppAD::link_dll_lib* dll_linker_ptr =
         new CppAD::link_dll_lib(dll_file, err_msg);
      if( err_msg != "" )
      {  std::cerr << "link_det_minor: err_msg = " << err_msg << "\n";
         delete dll_linker_ptr;
         return nullptr;
      }
      return dll_linker_ptr;
   }
}

bool link_det_minor(
   const std::string&         job      ,
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &matrix   ,
   CppAD::vector<double>     &gradient )
{  CPPAD_ASSERT_UNKNOWN( matrix.size() == size * size );
   CPPAD_ASSERT_UNKNOWN( gradient.size() == size * size );
   // --------------------------------------------------------------------
   // check global options
   const char* valid[] = { "onetape", "optimize"};
   size_t n_valid = sizeof(valid) / sizeof(valid[0]);
   typedef std::map<string, bool>::iterator iterator;
   //
   for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
   {  if( itr->second )
      {  bool ok = false;
         for(size_t i = 0; i < n_valid; i++)
            ok |= itr->first == valid[i];
         if( ! ok )
            return false;
      }
   }
   // --------------------------------------------------------------------
   // pointer to dll linker
   static CppAD::link_dll_lib* static_dll_linker = nullptr;
   //
   // pointer to gradient_det function
   static CppAD::jit_double static_gradient_det;
   //
   // size corresponding static_fun
   static size_t static_size = 0;
   //
   // number of independent variables
   size_t nx = size * size;
   //
   // onetape
   bool onetape = global_option["onetape"];
   // ----------------------------------------------------------------------
   if( job == "setup" )
   {  if( onetape )
      {  if( static_dll_linker != nullptr )
            delete static_dll_linker;
         static_dll_linker = setup(size);
         //
         static_gradient_det = get_function_ptr(static_dll_linker);
         static_size         = size;
         if( static_gradient_det == nullptr )
            return false;
      }
      else
      {  static_gradient_det = nullptr;
         static_size         = 0;
      }
      return true;
   }
   if( job ==  "teardown" )
   {  if( static_dll_linker != nullptr )
      {  delete static_dll_linker;
         static_dll_linker = nullptr;
      }
      return true;
   }
   // -----------------------------------------------------------------------
   CPPAD_ASSERT_UNKNOWN( job == "run" );
   if( onetape ) while(repeat--)
   {  // use if before assert to avoid warning that static_size is not used
      if( size != static_size )
      {  CPPAD_ASSERT_UNKNOWN( size == static_size );
      }

      // get next matrix
      CppAD::uniform_01(nx, matrix);

      // evaluate the gradient
      size_t compare_change = 0;
      static_gradient_det(
         nx, matrix.data(), nx, gradient.data(), &compare_change
      );
   }
   else while(repeat--)
   {  if( static_dll_linker != nullptr )
         delete static_dll_linker;
      static_dll_linker = setup(size);
      //
      static_gradient_det = get_function_ptr(static_dll_linker);
      static_size         = size;
      if( static_gradient_det == nullptr )
         return false;
      //
      // get next matrix
      CppAD::uniform_01(nx, matrix);

      // evaluate the gradient
      size_t compare_change = 0;
      static_gradient_det(
         nx, matrix.data(), nx, gradient.data(), &compare_change
      );
   }
   return true;
}
# endif // CPPAD_C_COMPILER_GNU_FLAGS || CPPAD_C_COMPILER_MSVC_FLAGS
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_jit_det_minor.cpp}
*/
