// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include <limits>
//
bool test_nan(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, con_op_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   //
   // nan
   double nan = std::numeric_limits<double>::quiet_NaN();
   //
   // tape
   tape_t<double> tape;
   size_t n_ind = 1;
   size_t index_of_nan = size_t ( tape.set_ind(n_ind) );
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // tape
   tape.record_con_op(5.0);               // not used
   dep_vec[0] =  tape.record_con_op(nan); // nan
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   ok &= tape.op_vec().size()  == 3;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.arg_vec().size() == 3;
   //
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = 0.0;
   tape.eval(trace, val_vec);
   //
   // renumber
   tape.renumber();
   //
   // dead_code
   tape.dead_code();
   //
   ok &= tape.op_vec().size()  == 1;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 1;
   //
   // val_vec
   val_vec.resize( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = 0.0;
   tape.eval(trace, val_vec);
   //
   // y
   Vector<double> y(1);
   dep_vec = tape.dep_vec();
   y[0] = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= std::isnan( y[0] );
   //
   return ok;
}
// END_C++
