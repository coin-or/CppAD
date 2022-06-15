/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <filesystem>
# include <cppad/cppad.hpp>

// CALL_CONVENTION, CALL_IMPORT
# ifdef _MSC_VER
# define CALL_CONVENTION __cdecl
# define CALL_IMPORT     __declspec(dllimport)
# define OBJ_EXT         ".obj"
# else
# define CALL_CONVENTION
# define CALL_IMPORT
# define OBJ_EXT         ".o"
# endif

//  RTD_LAZY, DIR_SEP, DLL_EXT, OBJ_EXT
# ifndef _WIN32
// dlopen, dlsym, dlerror, RTD_LAZY
# include <dlfcn.h>
# define DIR_SEP         '/'
# define DLL_EXT         ".so"
# else
# include <windows.h>
# define RTLD_LAZY       0
# define DIR_SEP         '\\'
# define DLL_EXT         ".dll"
# endif

namespace { // BEGIN_EMPTY_NAMESPACE
//
// cppad_forward_zero_double
extern "C" {
    CALL_IMPORT typedef int (CALL_CONVENTION *cppad_forward_zero_double)(
        size_t        call_id,
        size_t        nx,
        const double* x,
        size_t        ny,
        double*       y,
        size_t*       compare_change
    );
    //
    // cppad_forward_zero_float
    CALL_IMPORT typedef int (CALL_CONVENTION *cppad_forward_zero_float)(
        size_t       call_id,
        size_t       nx,
        const float* x,
        size_t       ny,
        float*       y,
        size_t*      compare_change
    );
}
//
// dlopen, dlsym, dlerror
# ifdef _WIN32
void* dlopen(const char *filename, int flag)
{   HINSTANCE hinstance = LoadLibrary(filename);
    return reinterpret_cast<void*>( hinstance );
}
void* dlsym(void* handle, const char* symbol)
{   HINSTANCE hinstance = reinterpret_cast<HINSTANCE>( handle );
    FARPROC   farproc   = GetProcAddress(hinstance, symbol);
    return reinterpret_cast<void*>( farproc );
}
int dlclose(void* handle)
{   HINSTANCE hinstance = reinterpret_cast<HINSTANCE>( handle );
    int result          = FreeLibrary(hinstance);
    return result;
}
const char* dlerror(void)
{   // should have a different result for each thread
    static char result[100];
    //
    DWORD dw        = GetLastError();
    std::string str = CppAD::to_string(dw);
    str             = "Microsoft GetLastError() = " + str;
    size_t  i = 0;
    while(i < 99 && i < str.size())
        result[i] = str[i];
    result[i] = '\0';
    //
    return result;
}
# endif
//
// create_dynamic_library
std::string create_dynamic_library(
    const std::string&            library_name,
    CppAD::vector< std::string >& library_csrc
)
{   // path
    using std::filesystem::path;
    //
    // ok
    bool ok = true;
    //
    // check the the std::system function exists
    int flag = std::system(nullptr);
    if( flag == 0 )
        return "";
    //
    // original_path
    path original_path = std::filesystem::current_path();
    //
    // tmp_dir_path
    path tmp_dir_path = std::filesystem::temp_directory_path();
    //
    // tmp_dir
    std::string tmp_dir = tmp_dir_path.string();
    if( tmp_dir.back() != DIR_SEP )
        tmp_dir += DIR_SEP;
    //
    // change into temporary directory
    std::filesystem::current_path( tmp_dir_path );
    //
    // dll_file
    std::string dll_file = library_name + DLL_EXT;
    //
    // csrc_files
    CppAD::vector< std::string > csrc_files( library_csrc.size() );
    //
    // i_csrc
    for(size_t i_csrc = 0; i_csrc < library_csrc.size(); ++i_csrc) if( ok )
    {   //
        // c_file
        std::string c_file =
            library_name + "_" + CppAD::to_string(i_csrc) +  ".c";
        //
        // write c_file
        std::ofstream file;
        file.open(c_file, std::ios::out);
        file << library_csrc[i_csrc];
        file.close();
        //
        // csrc_files
        csrc_files[i_csrc] = c_file;
    }
    std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files);
    if( err_msg != "" )
    {   std::cerr << err_msg << "\n";
        return "";
    }
    //
    // dll_file
    dll_file = tmp_dir + dll_file;
    //
    // change back to original directory
    std::filesystem::current_path( original_path );
    //
    return dll_file;
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
    {   type_y[0] = type_x[0];
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
    {   if( order_up != 0 )
            return false;;
        taylor_y[0] = 1.0 / taylor_x[0];
        return true;
    }
public:
    // forward_zero
    std::string forward_zero(void)
    {   std::string csrc =
            "# include <stddef.h>\n"
            "int cppad_forward_zero_" + name_ + "(\n";
        csrc +=R"_(
    size_t        call_id,
    size_t        nx,
    const double* x,
    size_t        ny,
    double*       y,
    size_t*       compare_change)
{   if( nx != 1 ) return 1;
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
{   if( x >= 0.0 ) return std::floor(x);
    return std::ceil(x);
}
CPPAD_DISCRETE_FUNCTION(float, integer)
std::string discrete_integer(void)
{   std::string csrc = R"_(
# include <math.h>
float cppad_discrete_integer(const float x)
{   if( x >= 0.0 ) return floor(x);
    return ceil(x);
}
)_";
    return csrc;
}
// --------------------------------------------------------------------------
bool simple_cases(void)
{   // ok
    bool ok = true;
    //
    // AD, NearEqual
    using CppAD::AD;
    using CppAD::NearEqual;
    //
    // eps99
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // funciton_name
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
    // librar_csrc
    CppAD::vector<std::string> library_csrc(1);
    std::map< std::string, std::string> options;
    std::stringstream ss;
    f.to_csrc(ss, options);
    library_csrc[0] = ss.str();
    //
    // os_file_name
    std::string dll_file_str = create_dynamic_library(
        library_name, library_csrc
    );
    if( dll_file_str == "" )
    {   std::cout << "Failed to create " << library_name << "\n";
        ok = false;
        return ok;
    }
    //
    // handle
    void* handle = dlopen(dll_file_str.c_str(), RTLD_LAZY);
    if( handle == nullptr )
    {   const char *errstr = dlerror();
        if( errstr != nullptr )
            std::cout << "Dynamic linking error = " << errstr << "\n";
        ok = false;
    }
    else
    {   // forward_zero
        std::string complete_name = "cppad_forward_zero_" + function_name;
        cppad_forward_zero_double forward_zero =
            reinterpret_cast<cppad_forward_zero_double>(
                dlsym(handle, complete_name.c_str() )
        );
        //
        // ok
        // no change
        size_t call_id = 0;
        CppAD::vector<double> x(nx), y(ny);
        x[0] = Value( ax[0] );
        x[1] = Value( ax[1] );
        for(size_t i = 0; i < ny; ++i)
            y[i] = std::numeric_limits<double>::quiet_NaN();
        size_t compare_change = 0;
        int flag = forward_zero(
            call_id, nx, x.data(), ny, y.data(), &compare_change
        );
        ok &= flag == 0;
        ok &= compare_change == 0;
        //
        // check
        for(size_t i = 0; i < ny; ++i)
        {   // std::cout << "y = " << y[i] << ", ay = " << ay[i] << "\n";
            ok &= CppAD::NearEqual( y[i], Value(ay[i]), eps99, eps99);
        }
    }
    dlclose(handle);
    return ok;
}
// --------------------------------------------------------------------------
bool compare_cases(void)
{   // ok
    bool ok = true;
    //
    // AD, NearEqual
    using CppAD::AD;
    //
    // funciton_name
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
    // library_csrc
    CppAD::vector<std::string> library_csrc(1);
    std::map< std::string, std::string> options;
    std::stringstream ss;
    f.to_csrc(ss, options);
    library_csrc[0] = ss.str();
    //
    // dll_file_str
    std::string dll_file_str = create_dynamic_library(
        library_name, library_csrc
    );
    if( dll_file_str == "" )
    {   std::cout << "Failed to create " << library_name << "\n";
        ok = false;
        return ok;
    }
    //
    // handle
    void* handle = dlopen(dll_file_str.c_str(), RTLD_LAZY);
    if( handle == nullptr )
    {   const char *errstr = dlerror();
        if( errstr != nullptr )
            std::cout << "Dynamic linking error = " << errstr << "\n";
        ok = false;
    }
    else
    {   // forward_zero
        std::string complete_name = "cppad_forward_zero_" + function_name;
        cppad_forward_zero_double forward_zero =
            reinterpret_cast<cppad_forward_zero_double>(
                dlsym( handle, complete_name.c_str() )
        );
        //
        // ok
        // no change
        size_t call_id = 0;
        CppAD::vector<double> x(nx), y(ny);
        x[0] = x0;
        for(size_t i = 0; i < ny; ++i)
            y[i] = std::numeric_limits<double>::quiet_NaN();
        size_t compare_change = 0;
        int flag = forward_zero(
            call_id, nx, x.data(), ny, y.data(), &compare_change
        );
        ok &= flag == 0;
        ok &= compare_change == 0;
        for(size_t i = 0; i < ny; ++i)
            ok &= y[i] == Value( ay[i] );
        //
        // ok
        // check all change
        x[0] = 2.0 * x0;
        flag = forward_zero(
            call_id, nx, x.data(), ny, y.data(), &compare_change
        );
        ok &= flag == 0;
        ok &= compare_change == 4;
        for(size_t i = 0; i < ny; ++i)
            ok &= y[i] == Value( ay[i] );
    }
    dlclose(handle);
    return ok;
}
// ---------------------------------------------------------------------------
bool atomic_case(void)
{   // ok
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
    {   au[0] = ax[j];
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
    // library_csrc
    CppAD::vector<std::string> library_csrc(2);
    std::map< std::string, std::string> options;
    options["type"] = "double";
    library_csrc[0] = reciprocal.forward_zero();
    std::stringstream ss;
    f.to_csrc(ss, options);
    library_csrc[1] = ss.str();
    //
    // dll_file_str
    std::string dll_file_str = create_dynamic_library(
        library_name, library_csrc
    );
    if( dll_file_str == "" )
    {   std::cout << "Failed to create " << library_name << "\n";
        ok = false;
        return ok;
    }
    //
    // handle
    void* handle = dlopen(dll_file_str.c_str(), RTLD_LAZY);
    if( handle == nullptr )
    {   const char *errstr = dlerror();
        if( errstr != nullptr )
            std::cout << "Dynamic linking error = " << errstr << "\n";
        ok = false;
    }
    else
    {   // forward_zero
        std::string complete_name = "cppad_forward_zero_" + function_name;
        cppad_forward_zero_double forward_zero =
            reinterpret_cast<cppad_forward_zero_double>(
                dlsym( handle, complete_name.c_str() )
        );
        //
        // ok
        // no change
        CppAD::vector<double> x(nx), y(ny);
        x[0] = x0;
        x[1] = x1;
        for(size_t i = 0; i < ny; ++i)
            y[i] = std::numeric_limits<double>::quiet_NaN();
        size_t call_id        = 0;
        size_t compare_change = 0;
        int flag = forward_zero(
            call_id, nx, x.data(), ny, y.data(), &compare_change
        );
        ok &= flag == 0;
        ok &= compare_change == 0;
        for(size_t i = 0; i < ny; ++i)
            ok &= y[i] == Value( ay[i] );
    }
    dlclose(handle);
    return ok;
}
// ---------------------------------------------------------------------------
bool discrete_case(void)
{   // ok
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
    // library_csrc
    CppAD::vector<std::string> library_csrc(2);
    std::map< std::string, std::string> options;
    options["type"] = "float";
    library_csrc[0] = discrete_integer();
    std::stringstream ss;
    f.to_csrc(ss, options);
    library_csrc[1] = ss.str();
    //
    // dll_file_str
    std::string dll_file_str = create_dynamic_library(
        library_name, library_csrc
    );
    if( dll_file_str == "" )
    {   std::cout << "Failed to create " << library_name << "\n";
        ok = false;
        return ok;
    }
    //
    // handle
    void* handle = dlopen(dll_file_str.c_str(), RTLD_LAZY);
    if( handle == nullptr )
    {   const char *errstr = dlerror();
        if( errstr != nullptr )
            std::cout << "Dynamic linking error = " << errstr << "\n";
        ok = false;
    }
    else
    {   // forward_zero
        std::string complete_name = "cppad_forward_zero_" + function_name;
        cppad_forward_zero_float forward_zero =
            reinterpret_cast<cppad_forward_zero_float>(
                dlsym( handle, complete_name.c_str() )
        );
        //
        // ok
        // no change
        CppAD::vector<float> x(nx), y(ny);
        x[0] = x0;
        x[1] = x1;
        for(size_t i = 0; i < ny; ++i)
            y[i] = std::numeric_limits<float>::quiet_NaN();
        size_t call_id        = 0;
        size_t compare_change = 0;
        int flag = forward_zero(
            call_id, nx, x.data(), ny, y.data(), &compare_change
        );
        ok &= flag == 0;
        ok &= compare_change == 0;
        for(size_t i = 0; i < ny; ++i)
            ok &= y[i] == Value( ay[i] );
    }
    dlclose(handle);
    return ok;
}
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
bool to_csrc(void)
{   bool ok = true;
    ok     &= simple_cases();
    ok     &= compare_cases();
    ok     &= atomic_case();
    ok     &= discrete_case();
    return ok;
}
