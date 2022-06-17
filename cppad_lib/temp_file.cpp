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
$begin temp_file$$

$section Create a New Empty Temporary File$$

$head Syntax$$
$icode%file_name% = temp_file()
%$$

$head file_name$$
Is the name of a new temporary file.
This file did not exist when $code temp_file$$ was called.
It has been created and is empty upon return.

$subhead Empty$$
This routine failed if the reutrn value $icode file_name$$ is empty.

$head Thread Safe$$
This routine is thread safe when C++17 and $code mkstemp$$ are available
to CppAD.

$end
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

# if CPPAD_HAS_MKSTEMP && CPPAD_USE_CPLUSPLUS_2017
std::string temp_file(void)
{   // path
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
# else
std::string temp_file(void)
{   std::string file_name = tmpnam(nullptr);
    FILE* fp = fopen(file_name.c_str(), "w");
    if( fp == NULL )
        return "";
    fclose(fp);
    return file_name;
}
# endif

} } // END_CPPAD_LOCAL_NAMESPACE
