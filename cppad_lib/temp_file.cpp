// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin temp_file dev}
{xrst_spell
   mkstemp
}

Create a New Empty Temporary File
#################################

Syntax
******

   *file_name* = ``temp_file`` ()

file_name
*********
Is the name of a new temporary file.
This file did not exist when ``temp_file`` was called.
It has been created and is empty upon return.

Empty
=====
This routine failed if the return value *file_name* is empty.

Thread Safe
***********
This routine is thread safe when C++17 and ``mkstemp`` are available
to CppAD.

{xrst_end temp_file}
*/
# include <vector>
# include <stdio.h>
# include <cppad/configure.hpp>
# include <cppad/local/temp_file.hpp>

# if CPPAD_HAS_MKSTEMP && CPPAD_USE_CPLUSPLUS_2017
# include <filesystem>
# else
# include <stdio.h>
# endif

# if _MSC_VER
# include <io.h>
# else
# include <unistd.h>
# endif

# ifdef _WIN32
# define DIR_SEP '\\'
# else
# define DIR_SEP '/'
# endif

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE

std::string temp_file(void)
# if CPPAD_HAS_MKSTEMP && CPPAD_USE_CPLUSPLUS_2017
{  // path
   using std::filesystem::path;
   //
   // tmp_dir_path
   path tmp_dir_path = std::filesystem::temp_directory_path();
   //
   // tmp_dir_str
   std::string tmp_dir_str = tmp_dir_path.string();
   if( tmp_dir_str.back() != DIR_SEP )
      tmp_dir_str += DIR_SEP;
   //
   // pattern_str
   std::string pattern_str = tmp_dir_str;
   pattern_str            += "fileXXXXXX";
   //
   // pattern_vec
   std::vector<char> pattern_vec( pattern_str.size() + 1 );
   for(size_t i = 0; i < pattern_str.size(); ++i)
      pattern_vec[i] = pattern_str[i];
   pattern_vec[ pattern_str.size() ] = '\0';
   //
   // fd, pattrern_vec
   int fd = mkstemp( pattern_vec.data() );
   if( fd < 0 )
      return "";
   close(fd);
   //
   // file_name
   std::string file_name = pattern_vec.data();
   return file_name;
}
# else // CPPAD_HAS_MKSTEMP && CPPAD_USE_CPLUSPLUS_2017
{
# if CPPAD_HAS_TMPNAM_S
   char c_str[L_tmpnam_s];
   tmpnam_s(c_str, L_tmpnam_s );
# else
   char c_str[L_tmpnam];
   tmpnam(c_str);
# endif
# ifdef __MINGW32__
   // https://stackoverflow.com/questions/38868858/
   // fopen-of-file-name-created-by-tmpnam-fails-on-mingw
   std::string file_name = c_str + 1;
# else
   std::string file_name = c_str;
# endif
   FILE* fp = fopen(file_name.c_str(), "r");
   if( fp != NULL )
      return "";
   fp = fopen(file_name.c_str(), "w");
   if( fp == NULL )
      return "";
   fclose(fp);
   return file_name;
}
# endif // CPPAD_HAS_MKSTEMP && CPPAD_USE_CPLUSPLUS_2017

} } // END_CPPAD_LOCAL_NAMESPACE
