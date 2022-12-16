// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <fstream>
# include <cppad/local/temp_file.hpp>

bool temp_file(void)
{  bool ok = true;
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
