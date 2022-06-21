# ifndef CPPAD_UTILITY_CREATE_DLL_LIB_HPP
# define CPPAD_UTILITY_CREATE_DLL_LIB_HPP
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
$begin create_dll_lib$$
$spell
    CppAD
    std
    cppad
    dll
    hpp
    csrc
    Hs
    Hc
    gcc
$$

$section Create a Dynamic Link Library$$

$head Syntax$$
$codei%# include <cppad/utility/create_dll_lib.hpp>
%$$
$icode%err_msg% = create_dll_lib(%dll_file%, %csrc_files%, %options%)
%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_CREATE_DLL_LIB%// END_CREATE_DLL_LIB%1%$$

$head include$$
As with all the CppAD utilities, $code create_dll_lib.hpp$$ is included
by $code <cppad/cppad.hpp>$$, and can also be included separately.

$head dll_file$$
This is the file where the dynamic link library file named $icode dll_file$$.
This file name must have the proper extension for a dynamic link library
($code .so$$ on unix and $code .dll$$ on windows).

$head StringVector$$
The type $icode StringVector$$ is a simple vector with elements
of type $code std::string$$.

$head csrc_files$$
The vector $icode csrc_files$$ contains the names of the C source
files that are compiled and linked to the library.
These files do not have to have a specific extension.

$head options$$
The possible keys in this map are documented below.
The default value for each key is used when the key
does not appear in $icode options$$.

$subhead compile$$
This is an abbreviated version of the compile command.
It does not include the output file flag or output file name.
If $code _MSC_VER$$ is defined, the default value for this option is
$code cl /EHs /EHc /c /LD /TC$$
If $code _MSC_VER$$ is not defined, the default value for this option is
$code gcc -c -fPIC$$.

$subhead link$$
This is an abbreviated version of the link command.
It does not include the output file flag or output file name.
If $code _MSC_VER$$ is defined, the default value for this option is
$code link /DLL$$
If $code _MSC_VER$$ is not defined, the default value for this option is
$code gcc -shared$$.


$head err_msg$$
If this string is empty, no error occurred.
Otherwise the processing aborted and $icode err_msg$$ is the corresponding
error message.

$head Example$$
The file $cref dll_lib.cpp$$ contains an example and test of
$code create_dll_lib$$.

$end
*/
# include <map>
# include <cppad/local/temp_file.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// create
// BEGIN_CREATE_DLL_LIB
template <class StringVector>
std::string create_dll_lib(
    const std::string&                        dll_file   ,
    const StringVector&                       csrc_files ,
    const std::map<std::string, std::string>& options    )
// END_CREATE_DLL_LIB
{   using std::string;
    //
    // err_msg
    string err_msg = "";
    //
    // compile, link
# ifdef _MSC_VER
    string compile = "cl /EHs /EHc /c /LD /TC";
    string link    = "link /DLL";
# else
    string compile = "gcc -c -fPIC";
    string link    = "gcc -shared";
# endif
    for( const auto& pair : options )
    {   const string& key = pair.first;
        if( key == "compile" )
            compile = pair.second;
        else if( key == "link" )
            link = pair.second;
        else
        {   err_msg = "options contains following invalid key: " + key;
            return err_msg;
        }
    }
    //
    // check the std::system function exists
    int flag = std::system(nullptr);
    if( flag == 0 )
    {   err_msg = "C++ std::system function not availablee\n";
        return err_msg;
    }
    //
    // check the file extensions
# ifdef _WIN32
    string dll_ext = ".dll";
# else
    string dll_ext = ".so";
# endif
    size_t last_match = dll_file.rfind(dll_ext);
    size_t expected   = dll_file.size() - dll_ext.size();
    if( last_match != expected )
    {   err_msg += "dll_file = " + dll_file + "\ndoes not end with " + dll_ext;
        return err_msg;
    }
    //
    // o_file_list, o_file_vec;
    string       o_file_list;
    StringVector o_file_vec( csrc_files.size() );
    //
    // i_csrc
    for(size_t i_csrc = 0; i_csrc < csrc_files.size(); ++i_csrc)
    {   //
        // c_file
        string c_file = csrc_files[i_csrc];
        //
        // o_file
        string o_file = local::temp_file();
        //
        // cmd
        string cmd = compile + " " + c_file;
# ifdef _MSC_VER
        cmd += " /Fo\"" + o_file + "\" 1> nul 2> nul";
# else
        cmd += " -o " + o_file;
# endif
        //
        // o_file
        // compile c_file and put result in o_file
        flag = std::system( cmd.c_str() );
        if(  flag != 0 )
        {   err_msg = "create_dll_lib: following system comamnd failed\n";
            err_msg += cmd;
            return err_msg;
        }
        //
        // o_file_list
        o_file_list += " " + o_file;
        //
        // o_file_vec
        o_file_vec[i_csrc] = o_file;
    }
    string cmd = link + " " + o_file_list;
# ifdef _MSC_VER
    cmd += " /OUT:" + dll_file + " 1> nul 2> nul";
# else
    cmd += " -o "   + dll_file;;
# endif
    flag = std::system( cmd.c_str() );
    if(  flag != 0 )
    {   err_msg = "create_dll_lib: following system comamnd failed\n";
        err_msg += cmd;
        return err_msg;
    }
    //
    // remove o_file
    for(size_t i = 0; i < o_file_vec.size(); ++i)
    {   flag = std::remove( o_file_vec[i].c_str() );
        if(  flag != 0 )
        {   err_msg = "create_dll_lib: following system comamnd failed\n";
            err_msg += cmd;
            return err_msg;
        }
    }
    return err_msg;
}

} // END_CPPAD_NAMESPACE

# endif
