// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
namespace { // BEIGN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
bool con_op(void)
{  bool ok = true;
   //
   // AD, tape_t, Vector, addr_t, con_op_enum, add_op_enum;
   using CppAD::AD;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   //
   // tape
   tape_t< AD<double> > tape;
   addr_t n_ind = 1;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // tape
   addr_t five     = tape.record_con_op(5.0); // 5.0
   tape.record_con_op(5.0);                   // not used
   op_arg[0] = 0; // x[0]
   op_arg[1] = five;
   dep_vec[0] = tape.record_op(add_op_enum, op_arg); // x[0] + 5.0
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector< AD<double> > ax(1);
   ax[0] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector< AD<double> > val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = ax[i];
   tape.eval(trace, val_vec);
   //
   // ok
   ok &= tape.n_op()  == 4;
   ok &= tape.arg_vec().size() == 5;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.n_val()          == n_ind + 4;
   //
   // renumber
   tape.renumber();
   //
   // ok
   ok &= tape.n_op()  == 4;
   ok &= tape.arg_vec().size() == 5;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.n_val()          == n_ind + 4;
   //
   // dead_code
   tape.dead_code();
   //
   // val_vec
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   dep_vec =  tape.dep_vec();
   //
   // ok
   ok &= tape.n_op()  == 3;
   ok &= tape.arg_vec().size() == 4;
   ok &= tape.con_vec().size() == 2;
   ok &= tape.n_val()          == n_ind + 3;
   //
   // y
   Vector< AD<double> > ay(1);
   ay[0] = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= ay[0] == ax[0] + 5.0;
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_ad_double(void)
{  bool ok = true;
   ok     &= con_op();
   return ok;
}
