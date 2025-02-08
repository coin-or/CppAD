# ifndef CPPAD_CORE_OPTIMIZE_HPP
# define CPPAD_CORE_OPTIMIZE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# define CPPAD_CORE_OPTIMIZE_PRINT_RESULT 0

/*
{xrst_begin optimize}
{xrst_spell
   onetape
   substring
}

Optimize an ADFun Object Tape
#############################

Syntax
******
| *f* . ``optimize`` ()
| *f* . ``optimize`` ( *options* )
| *flag* = *f* . ``exceed_collision_limit`` ()

Purpose
*******
The operation sequence corresponding to an :ref:`ADFun-name` object can
be very large and involve many operations; see the
size functions in :ref:`fun_property-name` .
The *f* . ``optimize`` procedure reduces the number of operations,
and thereby the time and the memory, required to
compute function and derivative values.

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

options
*******
This argument has prototype

   ``const std::string&`` *options*

The default for *options* is the empty string.
If it is present, it must consist of one or more of the options below
separated by a single space character.

no_conditional_skip
===================
The ``optimize`` function can create conditional skip operators
to improve the speed of conditional expressions; see
:ref:`CondExp@Optimize` .
If the sub-string ``no_conditional_skip`` appears in *options* ,
conditional skip operations are not be generated.
This may make the optimize routine use significantly less memory
and take less time to optimize *f* .
If conditional skip operations are generated,
it may save a significant amount of time when
using *f* for :ref:`forward-name` or :ref:`reverse-name` mode calculations;
see :ref:`number_skip-name` .

no_compare_op
=============
If the sub-string ``no_compare_op`` appears in *options* ,
comparison operators will be removed from the optimized function.
These operators are necessary for the
:ref:`compare_change-name` functions to be meaningful.
On the other hand, they are not necessary, and take extra time,
when the compare_change functions are not used.

no_print_for_op
===============
If the sub-string ``no_compare_op`` appears in *options* ,
:ref:`PrintFor-name` operations will be removed form the optimized function.
These operators are useful for reporting problems evaluating derivatives
at independent variable values different from those used to record a function.

no_cumulative_sum_op
====================
If this sub-string appears,
no cumulative sum operations will be generated during the optimization; see
:ref:`optimize_cumulative_sum.cpp-name` .

collision_limit=value
=====================
If this substring appears,
where *value* is a sequence of decimal digits,
the optimizer's hash code collision limit will be set to *value* .
When the collision limit is reached, the expressions with that hash code
are removed and a new lists of expressions with that has code is started.
The larger *value* , the more identical expressions the optimizer
can recognize, but the slower the optimizer may run.
The default for *value* is ``10`` .

val_graph
=========
If the sub-string ``val_graph`` appears in *options* ,
the value graph optimizer is used.
This is a new (experimental) CppAD operation sequence optimizer.

#. The val_graph optimizer has a much simpler implementation.
#. It has better developer documentation
#. It has examples and tests at the val_graph level.
   This makes it easy to change the val_graph optimizer.
#. The optimized tape has very similar speed to the old optimizer; i.e.,
   when the :ref:`speed_main@Global Options@onetape` option is present.
   For some of the :ref:`speed-name` test case
   the val_graph optimized tape is significantly faster.
#. The val_graph optimizer take much longer to run.
   This is probably due to the conversion to and from a val_graph.

no_conditional_skip
-------------------
If the sub-string ``val_graph`` is present, the ``no_conditional_skip``
sub-string must also appear.

collision_limit=value
---------------------
If the sub-string ``val_graph`` is present, the ``collision_limit=value``
sub-string must **not** appear.
Currently, there is no collision limit for the new optimizer.

Re-Optimize
***********
Before 2019-06-28, optimizing twice was not supported and would fail
if cumulative sum operators were present after the first optimization.
This is now supported but it is not expected to have much benefit.
If you find a case where it does have a benefit, please inform the CppAD
developers of this.

Efficiency
**********
If a :ref:`zero order forward<forward_zero-name>` calculation is done during
the construction of *f* , it will require more memory
and time than required after the optimization procedure.
In addition, it will need to be redone.
For this reason, it is more efficient to use

| |tab| ``ADFun`` < *Base* > *f* ;
| |tab| *f* . ``Dependent`` ( *x* , *y* );
| |tab| *f* . ``optimize`` ();

instead of

| |tab| ``ADFun`` < *Base* > *f* ( *x* , *y* )
| |tab| *f* . ``optimize`` ();

See the discussion about
:ref:`sequence constructors<fun_construct@Sequence Constructor>` .

Taylor Coefficients
*******************
Any Taylor coefficients in the function object are lost; i.e.,
:ref:`f.size_order()<size_order-name>` after the optimization is zero.
(See the discussion about efficiency above.)

Speed Testing
*************
You can run the CppAD :ref:`speed<speed_main-name>` tests and see
the corresponding changes in number of variables and execution time.
Note that there is an interaction between using
:ref:`speed_main@Global Options@optimize` and
:ref:`speed_main@Global Options@onetape` .
If *onetape* is true and *optimize* is true,
the optimized tape will be reused many times.
If *onetape* is false and *optimize* is true,
the tape will be re-optimized for each test.

Atomic Functions
****************
There are some subtitle issue with optimized :ref:`atomic-name` functions
:math:`v = g(u)`:

rev_sparse_jac
==============
The :ref:`atomic_two_rev_sparse_jac-name` function is be used to determine
which components of *u* affect the dependent variables of *f* .
For each atomic operation, the current
:ref:`atomic_two_option@atomic_sparsity` setting is used
to determine if ``pack_sparsity_enum`` , ``bool_sparsity_enum`` ,
or ``set_sparsity_enum`` is used to determine dependency relations
between argument and result variables.

nan
===
If *u* [ *i* ] does not affect the value of
the dependent variables for *f* ,
the value of *u* [ *i* ] is set to :ref:`nan-name` .

Checking Optimization
*********************
If :ref:`Faq@Speed@NDEBUG` is not defined,
and :ref:`f.size_order()<size_order-name>` is greater than zero,
a :ref:`forward_zero-name` calculation is done using the optimized version
of *f* and the results are checked to see that they are
the same as before.
If they are not the same, the
:ref:`ErrorHandler-name` is called with a known error message
related to *f* . ``optimize`` () .

exceed_collision_limit
**********************
If the return value *flag* is true (false),
the previous call to *f* . ``optimize`` exceed the
:ref:`collision_limit<optimize@options@collision_limit=value>` .

Examples
********
{xrst_comment childtable without Example instead of Contents for header}
{xrst_toc_hidden
   example/optimize/optimize_twice.cpp
   example/optimize/forward_active.cpp
   example/optimize/reverse_active.cpp
   example/optimize/compare_op.cpp
   example/optimize/print_for.cpp
   example/optimize/conditional_skip.cpp
   example/optimize/nest_conditional.cpp
   example/optimize/cumulative_sum.cpp
}

.. csv-table::
   :widths: auto

   optimize_twice.cpp,:ref:`optimize_twice.cpp-title`
   optimize_forward_active.cpp,:ref:`optimize_forward_active.cpp-title`
   optimize_reverse_active.cpp,:ref:`optimize_reverse_active.cpp-title`
   optimize_compare_op.cpp,:ref:`optimize_compare_op.cpp-title`
   optimize_print_for.cpp,:ref:`optimize_print_for.cpp-title`
   optimize_conditional_skip.cpp,:ref:`optimize_conditional_skip.cpp-title`
   optimize_nest_conditional.cpp,:ref:`optimize_nest_conditional.cpp-title`
   optimize_cumulative_sum.cpp,:ref:`optimize_cumulative_sum.cpp-title`

{xrst_end optimize}
-----------------------------------------------------------------------------
*/
# include <cppad/local/optimize/optimize_run.hpp>
/*!
\file optimize.hpp
Optimize a player object operation sequence
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
Optimize a player object operation sequence

The operation sequence for this object is replaced by one with fewer operations
but the same funcition and derivative values.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type
 Base.

\param options
\li
If the sub-string "no_conditional_skip" appears,
conditional skip operations will not be generated.
This may make the optimize routine use significantly less memory
and take significantly less time.
\li
If the sub-string "no_compare_op" appears,
then comparison operators will be removed from the optimized tape.
These operators are necessary for the compare_change function to be
be meaningful in the resulting recording.
On the other hand, they are not necessary and take extra time
when compare_change is not used.
*/
template <class Base, class RecBase>
void ADFun<Base,RecBase>::optimize(const std::string& options)
{
# if CPPAD_CORE_OPTIMIZE_PRINT_RESULT
   // size of operation sequence before optimizatiton
   size_t size_op_before = size_op();
# endif

   // number of independent variables
   size_t n_ind_var = ind_taddr_.size();

# ifndef NDEBUG
   // n_ind_dyn, ind_dynamic
   size_t n_ind_dyn = play_.num_dynamic_ind();
   CppAD::vector<Base> ind_dynamic(n_ind_dyn);
   //
   // n_dep_var, x, y, check, max_taylor, check_zero_order
   size_t n_dep_var = dep_taddr_.size();
   CppAD::vector<Base> x(n_ind_var), y(n_dep_var), check(n_dep_var);
   Base max_taylor(0);
   bool check_zero_order = num_order_taylor_ > 0;
   if( check_zero_order )
   {  //
      // ind_dynamic
      for(size_t j = 0; j < n_ind_dyn; ++j)
      {  const addr_t par_ind = play_.dyn_ind2par_ind()[j];
         ind_dynamic[j]       = play_.all_par_vec()[par_ind];
      }
      //
      // x
      // zero order coefficients for independent vars
      for(size_t j = 0; j < n_ind_var; j++)
      {  CPPAD_ASSERT_UNKNOWN( play_.GetOp(j+1) == local::InvOp );
         CPPAD_ASSERT_UNKNOWN( ind_taddr_[j]    == j+1   );
         x[j] = taylor_[ ind_taddr_[j] * cap_order_taylor_ + 0];
      }
      // y
      // zero order coefficients for dependent vars
      for(size_t i = 0; i < n_dep_var; i++)
      {  CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_  );
         y[i] = taylor_[ dep_taddr_[i] * cap_order_taylor_ + 0];
      }
      // max_taylor
      // maximum zero order coefficient not counting BeginOp at beginning
      // (which is correpsonds to uninitialized memory).
      for(size_t i = 1; i < num_var_tape_; i++)
      {  if(  abs_geq(taylor_[i*cap_order_taylor_+0] , max_taylor) )
            max_taylor = taylor_[i*cap_order_taylor_+0];
      }
   }
# endif

   //
   // val_graph
   bool val_graph = options.find("val_graph") != std::string::npos;
   //
   if( val_graph )
   {  val_optimize(options);
      exceed_collision_limit_ = false;
   }
   else
   {
      // place to store the optimized version of the recording
      local::recorder<Base> rec;

      // create the optimized recording
      size_t exceed = false;
      switch( play_.address_type() )
      {
         case local::play::unsigned_short_enum:
         exceed = local::optimize::optimize_run<unsigned short>(
            options, n_ind_var, dep_taddr_, &play_, &rec
         );
         break;

         case local::play::unsigned_int_enum:
         exceed = local::optimize::optimize_run<unsigned int>(
            options, n_ind_var, dep_taddr_, &play_, &rec
         );
         break;

         case local::play::size_t_enum:
         exceed = local::optimize::optimize_run<size_t>(
            options, n_ind_var, dep_taddr_, &play_, &rec
         );
         break;

         default:
         CPPAD_ASSERT_UNKNOWN(false);
      }
      exceed_collision_limit_ = exceed;

      // now replace the recording
      play_.get_recording(rec, n_ind_var);
   }

   // number of variables in the recording
   num_var_tape_  = play_.num_var_rec();

   // set flag so this function knows it has been optimized
   has_been_optimized_ = true;

   // free memory allocated for sparse Jacobian calculation
   // (the results are no longer valid)
   for_jac_sparse_pack_.resize(0, 0);
   for_jac_sparse_set_.resize(0,0);

   // free old Taylor coefficient memory
   taylor_.clear();
   num_order_taylor_     = 0;
   cap_order_taylor_     = 0;

   // resize and initilaize conditional skip vector
   // (must use player size because it now has the recoreder information)
   cskip_op_.resize( play_.num_op_rec() );

   // resize subgraph_info_
   subgraph_info_.resize(
      ind_taddr_.size(),    // n_ind
      dep_taddr_.size(),    // n_dep
      play_.num_op_rec(),   // n_op
      play_.num_var_rec()   // n_var
   );

# ifndef NDEBUG
   if( check_zero_order )
   {  std::stringstream s;
      //
      // zero order forward calculation using new operation sequence
      new_dynamic(ind_dynamic);
      check = Forward(0, x, s);

      // check results
      Base eps99 = Base(99) * CppAD::numeric_limits<Base>::epsilon();
      for(size_t i = 0; i < n_dep_var; i++)
      if( ! abs_geq( eps99 * max_taylor , check[i] - y[i] ) )
      {  std::string msg = "Error during check of f.optimize().";
         msg += "\neps99 * max_taylor = " + to_string(eps99 * max_taylor);
         msg += "\ncheck[i] = " + to_string(check[i]);
         msg += "\ny[i]     = " + to_string(y[i]);
         CPPAD_ASSERT_KNOWN(
            abs_geq( eps99 * max_taylor , check[i] - y[i] ) ,
            msg.c_str()
         );
      }

      // Erase memory that this calculation was done so NDEBUG gives
      // same final state for this object (from users perspective)
      num_order_taylor_     = 0;
   }
# endif
# if CPPAD_CORE_OPTIMIZE_PRINT_RESULT
   // size of operation sequence after optimizatiton
   size_t size_op_after = size_op();
   std::cout << "optimize: size_op:  before = " <<
   size_op_before << ", after = " << size_op_after << "\n";
# endif
}

} // END_CPPAD_NAMESPACE

# undef CPPAD_CORE_OPTIMIZE_PRINT_RESULT
# endif
