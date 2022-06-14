/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <fstream>
# include <cppad/local/temp_file.hpp>

bool temp_file(void)
{   bool ok = true;
    //
    // file_name
    // create file
    std::string file_name = CppAD::local::temp_file();
    //
    // ifs
    std::ifstream ifs;
    ifs.open(file_name);
    //
    // ok
    ok   &= ifs.good();
    int c = ifs.get();
    ok   &= c == EOF;
    //
    // ok
    // remove file
    ifs.close();
    int flag = std::remove(file_name.c_str());
    ok &= flag == 0;
    //
    return ok;
}
