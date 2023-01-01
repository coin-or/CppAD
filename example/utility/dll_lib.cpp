/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
$begin dll_lib.cpp$$
$spell
    dll
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io%current documentation%$$.
$$
$section dll_lib: Example and Test$$

$head options$$
The following subsection of this example sets
$cref/options/create_dll_lib/options/$$ that are different
from the default options:
$srcthisfile%0%// BEGIN_OPTIONS%// END_OPTIONS%1%$$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <filesystem>
# include <fstream>
# include <vector>
# include <map>
# include <cppad/utility/create_dll_lib.hpp>
# include <cppad/utility/link_dll_lib.hpp>

# if _WIN32
# define DIR_SEP '\\'
# define DLL_EXT ".dll"
# else
# define DIR_SEP '/'
# define DLL_EXT ".so"
# endif

// CALL_CONVENTION, CALL_IMPORT
# ifdef _MSC_VER
# define CALL_CONVENTION __cdecl
# define CALL_IMPORT     __declspec(dllimport)
# else
# define CALL_CONVENTION
# define CALL_IMPORT
# endif

namespace {
    extern "C"
        typedef CALL_IMPORT int (CALL_CONVENTION *function_ptr)(int x, int y);
}



bool dll_lib(void)
{   bool ok = true;
    //
    // add_source
    std::string add_source =
        "int add(int x, int y)\n"
        "{   return x + y; }\n"
    ;
    //
    // dll_entry_source
    std::string dll_entry_source =
        "extern int add(int x, int y);"
# ifdef _MSC_VER
        "__declspec(dllexport) int _cdecl dll_entry(int x, int y)\n"
# else
        "int dll_entry(int x, int y)\n"
# endif
        "{   return add(x, y);}"
    ;
    //
    // temp_dir
    std::string temp_dir = std::filesystem::temp_directory_path().string();
    if( temp_dir.back() != DIR_SEP )
        temp_dir += DIR_SEP;
    //
    // ofs
    std::ofstream ofs;
    //
    // add_file
    std::string add_file = temp_dir + "add.c";
    ofs.open(add_file.c_str(), std::ofstream::out);
    ofs << add_source;
    ofs.close();
    //
    // dll_entry_file
    std::string dll_entry_file = temp_dir + "dll_entry.c";
    ofs.open(dll_entry_file.c_str(), std::ofstream::out);
    ofs << dll_entry_source;
    ofs.close();
    //
// BEGIN_OPTIONS
    // Example using options that are different from the default options
    std::map< std::string, std::string > options;
# ifdef _MSC_VER
    options["compile"] = "cl /EHs /EHc /c /LD /TC /O2";
# else
    options["compile"] = "gcc -c -fPIC -O2";
# endif
// END_OPTIONS
    //
    // dll_file
    std::vector< std::string > csrc_files(2);
    csrc_files[0] = add_file;
    csrc_files[1] = dll_entry_file;
    std::string dll_file = temp_dir + "dll_entry" + DLL_EXT;
    CppAD::create_dll_lib(dll_file, csrc_files, options);
    //
    // dll_linker
    std::string err_msg;
    CppAD::link_dll_lib dll_linker(dll_file, err_msg);
    if( err_msg != "" )
    {   std::cerr << "dll_lib error: " << err_msg << "\n";
        return false;
    }
    //
    // dll_entry
    void* vptr = dll_linker("dll_entry", err_msg);
    if( err_msg != "" )
    {   std::cerr << "dll_lib error: " << err_msg << "\n";
        return false;
    }
    function_ptr dll_entry = reinterpret_cast<function_ptr>(vptr);
    //
    // z = dll_entry(x, y)
    int x = 1, y = 2;
    int z = dll_entry(x, y);
    //
    // ok
    ok &= z == (x + y);
    //
    return ok;
}
// END C++
