// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>
# include <cppad/utility/link_dll_lib.hpp>

# if CPPAD_USE_CPLUSPLUS_2017
# include <filesystem>
# endif

//  DIR_SEP, DLL_EXT
# ifndef _WIN32
# define DIR_SEP         '/'
# define DLL_EXT         ".so"
# else
# define DIR_SEP         '\\'
# define DLL_EXT         ".dll"
# endif

namespace { // BEGIN_EMPTY_NAMESPACE
//
// dll_file_name
std::string dll_file_name(void)
{  //
# if ! CPPAD_USE_CPLUSPLUS_2017
   std::string tmp_dir = "./";
# else
   // tmp_dir
   std::string  tmp_dir = std::filesystem::temp_directory_path().string();
   if( tmp_dir.back() != DIR_SEP )
      tmp_dir += DIR_SEP;
# endif
   //
   // base_name
# ifdef _WIN32
   std::string base_name = "test_to_csrc.dll";
# else
   std::string base_name = "test_to_csrc.so";
# endif
   //
   // file_name
   std::string file_name = tmp_dir + base_name;
   //
   return file_name;
}
//
// create_csrc_file
std::string create_csrc_file(size_t index, const std::string& csrc)
{  //
# if ! CPPAD_USE_CPLUSPLUS_2017
   std::string tmp_dir = "./";
# else
   // tmp_dir
   std::string  tmp_dir = std::filesystem::temp_directory_path().string();
   if( tmp_dir.back() != DIR_SEP )
      tmp_dir += DIR_SEP;
# endif
   //
   // base_name
   std::string base_name = "test_to_csrc_" + CppAD::to_string(index) + ".c";
   //
   // file_name
   std::string file_name = tmp_dir + base_name;
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
// --------------------------------------------------------------------------
// integer
float integer(const float& x)
{  if( x >= 0.0 ) return std::floor(x);
   return std::ceil(x);
}
CPPAD_DISCRETE_FUNCTION(float, integer)
std::string discrete_integer(void)
{  std::string csrc = R"_(
# include <math.h>
float cppad_discrete_integer(const float x)
{  if( x >= 0.0 ) return floor(x);
   return ceil(x);
}
)_";
   return csrc;
}
// --------------------------------------------------------------------------
bool simple_cases(void)
{  // ok
   bool ok = true;
   //
   // AD, NearEqual
   using CppAD::AD;
   using CppAD::NearEqual;
   //
   // eps99
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   //
   // function_name
   std::string function_name = "test_to_csrc";
   //
   // library_name
   std::string library_name = function_name;
   //
   // nx, ax
   size_t nx = 2;
   CPPAD_TESTVECTOR( AD<double> ) ax(nx);
   ax[0] = 0.5;
   ax[1] = 2.0;
   CppAD::Independent(ax);
   //
   // ny, ay
   size_t ny = 28;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);
   //
   // binary operators
   ay[0] = ax[0] + ax[1]; // add
   ay[1] = ax[0] / ax[1]; // div
   ay[2] = ax[0] * ax[1]; // mul
   ay[3] = ax[0] - ax[1]; // sub
   //
   // unary functions
   ay[4]  = abs(   ax[0] );
   ay[5]  = acos(  ax[0] ); // ax[0] < 1
   ay[6]  = acosh( ax[1] ); // ax[1] > 1
   ay[7]  = asin(  ax[0] ); // ax[0] < 1
   ay[8]  = asinh( ax[0] );
   ay[9]  = atan(  ax[0] );
   ay[10] = atanh( ax[0] );
   ay[11] = cos(   ax[0] );
   ay[12] = cosh(  ax[0] );
   ay[13] = erf(   ax[0] );
   ay[14] = erfc(  ax[0] );
   ay[15] = exp(   ax[0] );
   ay[16] = expm1( ax[0] );
   ay[17] = log1p( ax[0] );
   ay[18] = log(   ax[0] );
   ay[19] = sign(  ax[0] );
   ay[20] = sin(   ax[0] );
   ay[21] = sinh(  ax[0] );
   ay[22] = sqrt(  ax[0] );
   ay[23] = tan(   ax[0] );
   ay[24] = tanh(  ax[0] );
   //
   // binary functions
   ay[25] = azmul( ax[0], ax[1] );
   ay[26] = pow(   ax[0], ax[1] ); // ax[0] > 0
   //
   // constant function
   ay[27] = 3.0;
   //
   // f
   CppAD::ADFun<double> f(ax, ay);
   f.function_name_set(function_name);
   //
   // dll_file
   std::string dll_file = dll_file_name();
   //
   // csrc_files
   CppAD::vector<std::string> csrc_files(1);
   std::string type = "double";
   std::stringstream ss;
   f.to_csrc(ss, type);
   csrc_files[0] = create_csrc_file(0, ss.str() );
   //
   // create dll_lib
   std::map< std::string, std::string > options;
   std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cout << err_msg << "\n";
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
   {  std::cout << "dll_linker ctor error: " << err_msg << "\n";
      ok = false;
   }
   else
   {  // jit_function
      std::string complete_name = "cppad_jit_" + function_name;
      jit_function = reinterpret_cast<jit_double>(
            dll_linker(complete_name, err_msg)
      );
      if( err_msg != "" )
      {  std::cout << "dll_linker fun_ptr error: " << err_msg << "\n";
         ok = false;
      }
   }
   if( ok )
   {  //
      // ok
      // no change
      CppAD::vector<double> x(nx), y(ny);
      x[0] = Value( ax[0] );
      x[1] = Value( ax[1] );
      for(size_t i = 0; i < ny; ++i)
         y[i] = std::numeric_limits<double>::quiet_NaN();
      size_t compare_change = 0;
      int flag = jit_function(
         nx, x.data(), ny, y.data(), &compare_change
      );
      ok &= flag == 0;
      ok &= compare_change == 0;
      //
      // check
      for(size_t i = 0; i < ny; ++i)
      {  // std::cout << "y = " << y[i] << ", ay = " << ay[i] << "\n";
         ok &= CppAD::NearEqual( y[i], Value(ay[i]), eps99, eps99);
      }
   }
   return ok;
}
// --------------------------------------------------------------------------
bool compare_cases(void)
{  // ok
   bool ok = true;
   //
   // AD, NearEqual
   using CppAD::AD;
   //
   // function_name
   std::string function_name = "test_to_csrc";
   //
   // library_name
   std::string library_name = function_name;
   //
   // nx, ax
   size_t nx = 1;
   CPPAD_TESTVECTOR( AD<double> ) ax(nx);
   double x0  = 0.5;
   ax[0] = x0;
   CppAD::Independent(ax);
   //
   // ny, ay
   size_t ny = 4;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);
   //
   // comp_eq_graph_op
   if( ax[0] == x0 )
      ay[0] = 1.0;
   else
      ay[0] = 0.0;
   //
   // comp_le_graph_op
   if( ax[0] <= x0 )
      ay[1] = 1.0;
   else
      ay[1] = 0.0;
   //
   // comp_lt_graph_op
   if( ax[0] <= x0 )
      ay[2] = 1.0;
   else
      ay[2] = 0.0;
   //
   // comp_ne_graph_op
   if( ax[0] != 2.0 * x0 )
      ay[3] = 1.0;
   else
      ay[3] = 0.0;
   //
   // f
   CppAD::ADFun<double> f(ax, ay);
   f.function_name_set(function_name);
   //
   // dll_file
   std::string dll_file = dll_file_name();
   //
   // csrc_files
   CppAD::vector<std::string> csrc_files(1);
   std::string type = "double";
   std::stringstream ss;
   f.to_csrc(ss, type);
   csrc_files[0] = create_csrc_file(0, ss.str());
   //
   // create_dll_lib
   std::map< std::string, std::string > options;
   std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cout << err_msg << "\n";
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
   {  std::cout << "dll_linker ctor error: " << err_msg << "\n";
      ok = false;
   }
   else
   {  // jit_function
      std::string complete_name = "cppad_jit_" + function_name;
      jit_function = reinterpret_cast<jit_double>(
            dll_linker(complete_name, err_msg)
      );
      if( err_msg != "" )
      {  std::cout << "dll_linker fun_ptr error: " << err_msg << "\n";
         ok = false;
      }
   }
   if( ok )
   {  //
      // ok
      // no change
      CppAD::vector<double> x(nx), y(ny);
      x[0] = x0;
      for(size_t i = 0; i < ny; ++i)
         y[i] = std::numeric_limits<double>::quiet_NaN();
      size_t compare_change = 0;
      int flag = jit_function(
         nx, x.data(), ny, y.data(), &compare_change
      );
      ok &= flag == 0;
      ok &= compare_change == 0;
      for(size_t i = 0; i < ny; ++i)
         ok &= y[i] == Value( ay[i] );
      //
      // ok
      // check all change
      x[0] = 2.0 * x0;
      flag = jit_function(
         nx, x.data(), ny, y.data(), &compare_change
      );
      ok &= flag == 0;
      ok &= compare_change == 4;
      for(size_t i = 0; i < ny; ++i)
         ok &= y[i] == Value( ay[i] );
   }
   return ok;
}
// ---------------------------------------------------------------------------
bool atomic_case(void)
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
   CPPAD_TESTVECTOR( AD<double> ) au(1), aw(1);
   for(size_t j = 0; j < nx; ++j)
   {  au[0] = ax[j];
      reciprocal(au, aw);
      ay[j] = aw[0];
   }
   //
   // function_name
   function_name = "use_reciprocal";
   //
   // f
   CppAD::ADFun<double> f(ax, ay);
   f.function_name_set(function_name);
   //
   // library_name
   std::string library_name = "test_to_csrc";
   //
   // dll_file
   std::string dll_file = dll_file_name();
   //
   // csrc_files
   CppAD::vector<std::string> csrc_files(2);
   csrc_files[0] = create_csrc_file(0, reciprocal.forward_zero() );
   std::string type = "double";
   std::stringstream ss;
   f.to_csrc(ss, type);
   csrc_files[1] = create_csrc_file(1, ss.str() );
   //
   // create_dll_lib
   std::map< std::string, std::string > options;
   std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cout << err_msg << "\n";
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
   {  std::cout << "dll_linker ctor error: " << err_msg << "\n";
      ok = false;
   }
   else
   {  // jit_function
      std::string complete_name = "cppad_jit_" + function_name;
      jit_function = reinterpret_cast<jit_double>(
            dll_linker(complete_name, err_msg)
      );
      if( err_msg != "" )
      {  std::cout << "dll_linker fun_ptr error: " << err_msg << "\n";
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
         ok &= y[i] == Value( ay[i] );
   }
   return ok;
}
// ---------------------------------------------------------------------------
bool discrete_case(void)
{  // ok
   bool ok = true;
   //
   // AD
   using CppAD::AD;
   //
   // nx, ax
   size_t nx = 2;
   CPPAD_TESTVECTOR( AD<float> ) ax(nx);
   float x0 = -1.5, x1 = 1.5;
   ax[0] = x0;
   ax[1] = x1;
   CppAD::Independent(ax);
   //
   // ny, ay
   size_t ny = nx;
   CPPAD_TESTVECTOR( AD<float> ) ay(ny);
   for(size_t j = 0; j < nx; ++j)
      ay[j] = integer( ax[j] );
   //
   // function_name
   std::string function_name = "use_integer";
   //
   // f
   CppAD::ADFun<float> f(ax, ay);
   f.function_name_set(function_name);
   //
   // library_name
   std::string library_name = "test_to_csrc";
   //
   // dll_file
   std::string dll_file = dll_file_name();
   //
   // csrc_files
   CppAD::vector<std::string> csrc_files(2);
   csrc_files[0] = create_csrc_file(0, discrete_integer() );
   std::string type = "float";
   std::stringstream ss;
   f.to_csrc(ss, type);
   csrc_files[1] = create_csrc_file(1, ss.str());
   //
   // dll_file_str
   std::map< std::string, std::string > options;
   std::string err_msg = create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cout << err_msg << "\n";
      ok = false;
      return ok;
   }
   //
   // dll_linker
   CppAD::link_dll_lib dll_linker(dll_file, err_msg);
   //
   // jit_float
   using CppAD::jit_float;
   //
   // jit_function
   jit_float jit_function = nullptr;
   if( err_msg != "" )
   {  std::cout << "dll_linker ctor error: " << err_msg << "\n";
      ok = false;
   }
   else
   {  // jit_function
      std::string complete_name = "cppad_jit_" + function_name;
      jit_function = reinterpret_cast<jit_float>(
            dll_linker(complete_name, err_msg)
      );
      if( err_msg != "" )
      {  std::cout << "dll_linker fun_ptr error: " << err_msg << "\n";
         ok = false;
      }
   }
   if( ok )
   {  //
      // ok
      // no change
      CppAD::vector<float> x(nx), y(ny);
      x[0] = x0;
      x[1] = x1;
      for(size_t i = 0; i < ny; ++i)
         y[i] = std::numeric_limits<float>::quiet_NaN();
      size_t compare_change = 0;
      int flag = jit_function(
         nx, x.data(), ny, y.data(), &compare_change
      );
      ok &= flag == 0;
      ok &= compare_change == 0;
      for(size_t i = 0; i < ny; ++i)
         ok &= y[i] == Value( ay[i] );
   }
   return ok;
}
// ---------------------------------------------------------------------------
bool csum_case(void)
{  // ok
   bool ok = true;
   //
   // AD
   using CppAD::AD;
   //
   // nx, ax
   size_t nx = 4;
   CPPAD_TESTVECTOR( AD<double> ) ax(nx);
   for(size_t j = 0; j < nx; ++j)
      ax[j] = 0.0;
   CppAD::Independent(ax);
   //
   // ny, ay
   size_t ny = 2;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);
   ay[0] = 0.0;
   ay[1] = 0.0;
   for(size_t j = 0; j < nx; ++j)
   {  ay[0] += ax[j];
      ay[1] -= ax[j];
   }
   //
   // function_name
   std::string function_name = "csum";
   //
   // f
   CppAD::ADFun<double> f(ax, ay);
   f.optimize();
   f.function_name_set(function_name);
   //
   // library_name
   std::string library_name = "test_to_csrc";
   //
   // dll_file
   std::string dll_file = dll_file_name();
   //
   // csrc_files
   CppAD::vector<std::string> csrc_files(1);
   std::string type = "double";
   std::stringstream ss;
   f.to_csrc(ss, type);
   csrc_files[0] = create_csrc_file(0, ss.str() );
   //
   // dll_file_str
   std::map< std::string, std::string > options;
   std::string err_msg = create_dll_lib(dll_file, csrc_files, options);
   if( err_msg != "" )
   {  std::cout << err_msg << "\n";
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
   {  std::cout << "dll_linker ctor error: " << err_msg << "\n";
      ok = false;
   }
   else
   {  // jit_function
      std::string complete_name = "cppad_jit_" + function_name;
      jit_function = reinterpret_cast<jit_double>(
            dll_linker(complete_name, err_msg)
      );
      if( err_msg != "" )
      {  std::cout << "dll_linker fun_ptr error: " << err_msg << "\n";
         ok = false;
      }
   }
   if( ok )
   {  //
      // ok
      // no change
      CppAD::vector<double> x(nx), y(ny);
      for(size_t j = 0; j < nx; ++j)
         x[j] = double(j+1);
      y[0] = std::numeric_limits<double>::quiet_NaN();
      y[1] = std::numeric_limits<double>::quiet_NaN();
      size_t compare_change = 0;
      int flag = jit_function(
         nx, x.data(), ny, y.data(), &compare_change
      );
      ok &= flag == 0;
      ok &= compare_change == 0;
      ok &= y[0] == double(( nx * (nx + 1) )  / 2 );
      ok &= y[1] == - double(( nx * (nx + 1) )  / 2 );
   }
   return ok;
}
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
bool to_csrc(void)
{  bool ok = true;
   ok     &= simple_cases();
   ok     &= compare_cases();
   ok     &= atomic_case();
   ok     &= discrete_case();
   ok     &= csum_case();
   return ok;
}
