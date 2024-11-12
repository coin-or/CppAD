# ifndef CPPAD_UTILITY_CREATE_DLL_LIB_HPP
# define CPPAD_UTILITY_CREATE_DLL_LIB_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin create_dll_lib}
{xrst_spell
   cl
   cmd
   csrc
   hc
   hs
   msg
}

Create a Dynamic Link Library
#############################

Syntax
******
| # ``include <cppad/utility/create_dll_lib.hpp>``
| *err_msg* = ``create_dll_lib`` ( *dll_file* , *csrc_files* , *options* )

Prototype
*********
{xrst_literal
   // BEGIN_CREATE_DLL_LIB
   // END_CREATE_DLL_LIB
}

include
*******
As with all the CppAD utilities, ``create_dll_lib.hpp`` is included
by ``<cppad/cppad.hpp>`` , and can also be included separately.

dll_file
********
This is the file where the dynamic link library file named *dll_file* .
This file name must have the proper extension for a dynamic link library
(``.so`` on unix and ``.dll`` on windows).

StringVector
************
The type *StringVector* is a simple vector with elements
of type ``std::string`` .

csrc_files
**********
The vector *csrc_files* contains the names of the C source
files that are compiled and linked to the library.
These files do not have to have a specific extension.

options
*******
The possible keys in this map are documented below.
The default value for each key is used when the key
does not appear in *options* .

compile
=======
This is an abbreviated version of the compile command.
It does not include the output file flag or output file name.
If :ref:`cmake-name` detects that this is the MSVC compiler,
the default value for this option is

   `cl /EHs /EHc /c /TC``

If cmake detects that this is the Clang or GNU compiler,
the default value for this option is

   *cppad_c_compiler_cmd* ``-c -fPIC``

Here and below *cppad_c_compiler_cmd* is the command used to run
the C compiler (which is determined by cmake) .


link
====
This is an abbreviated version of the link command.
It does not include the output file flag or output file name.
In the MSVC case, the default for this option is

   ``link /DLL``

In the Clang or GNU case, the default for this option is

   *cppad_c_compiler_cmd* ``-shared`` .


err_msg
*******
If this string is empty, no error occurred.
Otherwise the processing aborted and *err_msg* is the corresponding
error message.

Example
*******
The file :ref:`dll_lib.cpp-name` contains an example and test of
``create_dll_lib`` .

{xrst_end create_dll_lib}
*/
# include <map>
# include <cppad/local/temp_file.hpp>
# include <cppad/configure.hpp>

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
{  using std::string;
   //
   // err_msg
   string err_msg = "";
   //
   // compile, link
   string compile = "";
   string  link   = "";
# if CPPAD_C_COMPILER_MSVC_FLAGS
   compile = CPPAD_C_COMPILER_CMD " /EHs /EHc /c /TC";
   link    = "link /DLL";
# endif
# if CPPAD_C_COMPILER_GNU_FLAGS
   compile = CPPAD_C_COMPILER_CMD " -c -fPIC";
   link    = CPPAD_C_COMPILER_CMD " -shared";
# endif
   for( const auto& pair : options )
   {  const string& key = pair.first;
      if( key == "compile" )
         compile = pair.second;
      else if( key == "link" )
         link = pair.second;
      else
      {  err_msg = "options contains following invalid key: " + key;
         return err_msg;
      }
   }
   //
   // check if we know how to create a dll with this compiler
   if( compile == "" )
   {  err_msg  = "Do not know how to create a dll using this C compiler\n";
      err_msg += CPPAD_C_COMPILER_CMD;
      return err_msg;
   }
   //
   // check the std::system function exists
   int flag = std::system(nullptr);
   if( flag == 0 )
   {  err_msg = "C++ std::system function not availablee\n";
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
   {  err_msg += "dll_file = " + dll_file + "\ndoes not end with " + dll_ext;
      return err_msg;
   }
   //
   // o_file_list, o_file_vec;
   string       o_file_list;
   StringVector o_file_vec( csrc_files.size() );
   //
   // i_csrc
   for(size_t i_csrc = 0; i_csrc < csrc_files.size(); ++i_csrc)
   {  //
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
      {  err_msg = "create_dll_lib: following system command failed\n";
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
   {  err_msg = "create_dll_lib: following system command failed\n";
      err_msg += cmd;
      return err_msg;
   }
   //
   // remove o_file
   for(size_t i = 0; i < o_file_vec.size(); ++i)
   {  flag = std::remove( o_file_vec[i].c_str() );
      if(  flag != 0 )
      {  err_msg = "create_dll_lib: following system command failed\n";
         err_msg += cmd;
         return err_msg;
      }
   }
   return err_msg;
}

} // END_CPPAD_NAMESPACE

# endif
