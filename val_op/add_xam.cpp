// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include "tape.hpp"

bool add_xam()
{  bool ok = true;
   //
   // tape
   tape_t<double> tape;
   size_t n_ind = 2;
   tape.set_ind(n_ind);
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   op_arg[0] = 0; // x[0]
   op_arg[1] = 1; // x[1]
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // tape
   tape.next_op(add_op_enum, op_arg); // x[0] + x[1] (no used)
   tape.next_con_op(5.0);             // 5.0 (not used)
   //
   dep_vec[0]      = tape.next_op(add_op_enum, op_arg); // x[0] + x[1]
   op_arg[0]       = dep_vec[0];                        // x[0] + x[1]
   op_arg[1]       = tape.next_con_op(4.0);             // 4.0
   addr_t temp_1   = tape.next_op(sub_op_enum, op_arg); // x[0] + x[1] - 4.0
   op_arg[0]       = temp_1;
   op_arg[1]       = tape.next_con_op(4.0);
   dep_vec[1]      = tape.next_op(sub_op_enum, op_arg); // x[0] + x[1] - 8.0
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec;
   //
   // before optimizing
   val_vec.resize( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   ok &= tape.arg_vec().size() == 12;
   ok &= tape.con_vec().size() == 4;
   ok &= tape.op_vec().size() == 8;
   //
   // renumber
   tape.renumber();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // dead_code
   tape.dead_code();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // after optimizing
   ok &= tape.arg_vec().size() == 8;
   ok &= tape.con_vec().size() == 2;
   ok &= tape.op_vec().size() == 5;
   //
   // dep_vec
   dep_vec = tape.dep_vec();
   //
   // y
   Vector<double> y(2);
   for(size_t i = 0; i < 2; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[0] + x[1] - 8.0;
   //
   return ok;
}
