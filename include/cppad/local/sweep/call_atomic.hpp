# ifndef CPPAD_LOCAL_SWEEP_CALL_ATOMIC_HPP
# define CPPAD_LOCAL_SWEEP_CALL_ATOMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/atomic_index.hpp>
# include <cppad/core/atomic/two/atomic.hpp>
# include <cppad/core/atomic/three/atomic.hpp>
# include <cppad/core/atomic/four/atomic.hpp>

// BEGIN_CPAPD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {

/*
// ----------------------------------------------------------------------------
{xrst_begin_parent call_atomic dev}

Atomic Function Callbacks
#########################

{xrst_end call_atomic}
// ----------------------------------------------------------------------------
{xrst_begin atomic_forward_callback dev}

Forward Mode Callback to Atomic Functions
#########################################

Prototype
*********
{xrst_literal
   // BEGIN_FORWARD
   // END_FORWARD
}

Base
****
Is the base type corresponding to the atomic function call.

RecBase
*******
Is the base type corresponding to this atomic function call.

vector
******
is the CppAD::vector template class.

parameter_x
***********
contains the values, in afun(ax, ay), for arguments that are parameters.

type_x
******
what is the type, in the call, for each component of x.

need_y
******
specifies which components of taylor_y are necessary.
Only the components of y with the type equal to need_y need
to be computed. If need_y is greater than variable_enum,
then all the components of y must be computed.

select_y
********
specifies which components of taylor_y are necessary.

order_low
*********
lowest order for this forward mode calculation.

order_up
********
highest order for this forward mode calculation.

atom_index
**********
is the index, in local::atomic_index, corresponding to this atomic function.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

taylor_x
********
Taylor coefficients corresponding to x.

taylor_y
********
Taylor coefficient corresponding to y.

{xrst_end atomic_forward_callback}
*/
// BEGIN_FORWARD
template <class Base, class RecBase>
void call_atomic_forward(
   const vector<Base>&          parameter_x ,
   const vector<ad_type_enum>&  type_x      ,
   size_t                       need_y      ,
   const vector<bool>&          select_y    ,
   size_t                       order_low   ,
   size_t                       order_up    ,
   size_t                       atom_index  ,
   size_t                       call_id     ,
   const vector<Base>&          taylor_x    ,
   vector<Base>&                taylor_y    )
// END_FORWARD
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   bool         set_null = false;
   size_t       type     = 0;          // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if( ok )
   {
      if( type == 2 )
      {  atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
         afun->set_old(call_id);
         vector<ad_type_enum> empty;
         ok = afun->forward(
            order_low, order_up, empty, empty, taylor_x, taylor_y
         );
      }
      else if( type == 3 )
      {  CPPAD_ASSERT_UNKNOWN( type == 3 );
         atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
         ok = afun->forward(
            parameter_x, type_x,
            need_y, order_low, order_up, taylor_x, taylor_y
         );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<RecBase>* afun =
            reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
         ok = afun->forward(
            call_id, select_y, order_low, order_up, taylor_x, taylor_y
         );
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string name;
      local::atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic forward returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  atomic_base<RecBase>* afun =
         reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
      vector<ad_type_enum> empty;
      afun->set_old(call_id);
      afun->forward(
         order_low, order_up, empty, empty, taylor_x, taylor_y
      );
   }
   else if( type == 3 )
   {  atomic_three<RecBase>* afun =
         reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
      afun->forward(
         parameter_x, type_x,
         need_y, order_low, order_up, taylor_x, taylor_y
      );
   }
   else
   {  atomic_four<RecBase>* afun =
         reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
      afun->forward(
         call_id, select_y, order_low, order_up, taylor_x, taylor_y
      );
   }
# endif
}
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_reverse_callback dev}

Reverse Mode callback to Atomic Functions
#########################################

Prototype
*********
{xrst_literal
   // BEGIN_REVERSE
   // END_REVERSE
}

Base
****
Is the base type corresponding to the atomic function call.

RecBase
*******
Is the base type corresponding to this atomic function call.

vector
******
is the CppAD::vector template class.

parameter_x
***********
value of the parameter arguments to the atomic function
(other arguments have the value nan).

type_x
******
type for each component of x (not used by atomic_two interface).

select_x
********
specifies which components of partial_x are necessary.

order_up
********
highest order for this reverse mode calculation.

atom_index
**********
is the index, in local::atomic_index, corresponding to this atomic function.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

taylor_x
********
Taylor coefficients corresponding to x.

taylor_y
********
Taylor coefficient corresponding to y.

partial_x
*********
Partials w.r.t the x Taylor coefficients.

partial_y
*********
Partials w.r.t the y Taylor coefficients.

{xrst_end atomic_reverse_callback}
*/
// BEGIN_REVERSE
template <class Base, class RecBase>
void call_atomic_reverse(
   const vector<Base>&          parameter_x ,
   const vector<ad_type_enum>&  type_x      ,
   const vector<bool>&          select_x    ,
   size_t                       order_up    ,
   size_t                       atom_index  ,
   size_t                       call_id     ,
   const vector<Base>&          taylor_x    ,
   const vector<Base>&          taylor_y    ,
   vector<Base>&                partial_x   ,
   const vector<Base>&          partial_y   )
// END_REVERSE
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   bool         set_null = false;
   size_t       type     = 0;          // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if( ok )
   {
      if( type == 2 )
      {  atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
         afun->set_old(call_id);
         ok = afun->reverse(
            order_up, taylor_x, taylor_y, partial_x, partial_y
         );
      }
      else if( type == 3 )
      {  atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
         ok = afun->reverse(
            parameter_x, type_x,
            order_up, taylor_x, taylor_y, partial_x, partial_y
         );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<RecBase>* afun =
            reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
         ok = afun->reverse(
            call_id, select_x,
            order_up, taylor_x, taylor_y, partial_x, partial_y
         );
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string name;
      local::atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic reverse returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  atomic_base<RecBase>* afun =
         reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
      afun->set_old(call_id);
      afun->reverse(
         order_up, taylor_x, taylor_y, partial_x, partial_y
      );
   }
   else if( type == 3 )
   {  atomic_three<RecBase>* afun =
         reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
      afun->reverse(
         parameter_x, type_x,
         order_up, taylor_x, taylor_y, partial_x, partial_y
      );
   }
   else
   {  atomic_four<RecBase>* afun =
         reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
      afun->reverse(
         call_id, select_x,
         order_up, taylor_x, taylor_y, partial_x, partial_y
      );
   }
# endif
}
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_for_jac_sparsity_callback dev}

Forward Jacobian Sparsity Callback to Atomic Functions
######################################################

Prototype
*********
{xrst_literal
   // BEGIN_FOR_JAC_SPARSITY
   // END_FOR_JAC_SPARSITY
}

Base
****
Is the base type corresponding to the atomic function call.

vector
******
is the CppAD::vector template class.

InternalSparsity
****************
is the internal type used to represent sparsity; i.e.,
sparse::pack_setvec or sparse::list_setvec.

atom_index
**********
is the index, in local::atomic_index, corresponding to this atomic function.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

dependency
**********
is this a dependency or sparsity calculation.

parameter_x
***********
value of the parameter arguments to the atomic function
(other arguments have the value nan).

type_x
******
type for each component of x (not used by atomic_two interface).

x_index
*******
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.

y_index
*******
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.

var_sparsity
************
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the j-th argument to this atomic function.
On output, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th result for this atomic function.

{xrst_end atomic_for_jac_sparsity_callback}
*/
// BEGIN_FOR_JAC_SPARSITY
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_for_jac_sparsity(
   size_t                       atom_index    ,
   size_t                       call_id       ,
   bool                         dependency    ,
   const vector<Base>&          parameter_x   ,
   const vector<ad_type_enum>&  type_x        ,
   const vector<size_t>&        x_index       ,
   const vector<size_t>&        y_index       ,
   InternalSparsity&            var_sparsity  )
// END_FOR_JAC_SPARSITY
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   bool         set_null = false;
   size_t       type     = 0;       // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
   //
   // ident_zero_x
   vector<bool> ident_zero_x;
   if( type == 4 )
   {  size_t n = x_index.size();
      ident_zero_x.resize(n);
      for(size_t j = 0; j < n; ++j)
      {  if( type_x[j] > constant_enum )
            ident_zero_x[j] = false;
         else
            ident_zero_x[j] = IdenticalZero( parameter_x[j] );
      }
   }
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if ( ok )
   {
      if( type == 2 )
      {  atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
         afun->set_old(call_id);
         ok = afun->for_sparse_jac(
            parameter_x, x_index, y_index, var_sparsity
         );
      }
      else if( type == 3 )
      {  atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
         ok = afun->for_jac_sparsity(
         dependency, parameter_x, type_x, x_index, y_index, var_sparsity
         );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<RecBase>* afun =
            reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
         ok = afun->for_jac_sparsity( call_id,
            dependency, ident_zero_x, x_index, y_index, var_sparsity
         );
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string name;
      local::atomic_index<RecBase>(
         set_null, atom_index, type, &name, v_ptr
      );
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic jac_sparsity returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  atomic_base<RecBase>* afun =
         reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
      afun->set_old(call_id);
      afun->for_sparse_jac(
         parameter_x, x_index, y_index, var_sparsity
      );
   }
   else if( type == 3 )
   {  atomic_three<RecBase>* afun =
         reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
      afun->for_jac_sparsity(
         dependency, parameter_x, type_x, x_index, y_index, var_sparsity
      );
   }
   else
   {  atomic_four<RecBase>* afun =
         reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
      afun->for_jac_sparsity( call_id,
         dependency, ident_zero_x, x_index, y_index, var_sparsity
      );
   }
# endif
}
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_rev_jac_sparsity_callback dev}

Reverse Jacobian sparsity Callback to Atomic Functions
######################################################

Prototype
*********
{xrst_literal
   // BEGIN_REV_JAC_SPARSITY
   // END_REV_JAC_SPARSITY
}

Base
****
is the type corresponding to parameter_x
and to this atomic function.

InternalSparsity
****************
is the internal type used to represent sparsity; i.e.,
sparse::pack_setvec or sparse::list_setvec.

atom_index
**********
is the index, in local::atomic_index, corresponding to this atomic function.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

dependency
**********
is this a dependency or sparsity calculation.

parameter_x
***********
value of the parameter arguments to the atomic function
(other arguments have the value nan).

type_x
******
type for each component of x (not used by atomic_two interface).

x_index
*******
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.

y_index
*******
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.

var_sparsity [in/out]
*********************
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th argument to this atomic function.
On output, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
the sparsity has been updated to remove y as a function of x.

{xrst_end atomic_rev_jac_sparsity_callback}
*/
// BEGIN_REV_JAC_SPARSITY
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_rev_jac_sparsity(
   size_t                       atom_index    ,
   size_t                       call_id       ,
   bool                         dependency    ,
   const vector<Base>&          parameter_x   ,
   const vector<ad_type_enum>&  type_x        ,
   const vector<size_t>&        x_index       ,
   const vector<size_t>&        y_index       ,
   InternalSparsity&            var_sparsity  )
// END_REV_JAC_SPARSITY
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   bool         set_null = false;
   size_t       type     = 0;          // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
   //
   // ident_zero_x
   vector<bool> ident_zero_x;
   if( type == 4 )
   {  size_t n = x_index.size();
      ident_zero_x.resize(n);
      for(size_t j = 0; j < n; ++j)
      {  if( type_x[j] > constant_enum )
            ident_zero_x[j] = false;
         else
            ident_zero_x[j] = IdenticalZero( parameter_x[j] );
      }
   }
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if( ok )
   {
      if( type == 2 )
      {  atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
         afun->set_old(call_id);
         ok = afun->rev_sparse_jac(
            parameter_x, x_index, y_index, var_sparsity
         );
      }
      else if( type == 3 )
      {  atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
         ok = afun->rev_jac_sparsity(
         dependency, parameter_x, type_x, x_index, y_index, var_sparsity
         );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<RecBase>* afun =
            reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
         ok = afun->rev_jac_sparsity(call_id,
            dependency, ident_zero_x, x_index, y_index, var_sparsity
         );
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string name;
      local::atomic_index<RecBase>(
         set_null, atom_index, type, &name, v_ptr
      );
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic jac_sparsity returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  atomic_base<RecBase>* afun =
         reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
      afun->set_old(call_id);
      afun->rev_sparse_jac(
         parameter_x, x_index, y_index, var_sparsity
      );
   }
   else if( type == 3 )
   {  atomic_three<RecBase>* afun =
         reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
      afun->rev_jac_sparsity(
         dependency, parameter_x, type_x, x_index, y_index, var_sparsity
      );
   }
   else
   {  atomic_four<RecBase>* afun =
         reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
      afun->rev_jac_sparsity(call_id,
         dependency, ident_zero_x, x_index, y_index, var_sparsity
      );
   }
# endif
}
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_for_hes_sparsity_callback dev}
{xrst_spell
   np
}

Forward Hessian Sparsity Callback to Atomic Functions
#####################################################

Prototype
*********
{xrst_literal
   // BEGIN_FOR_HES_SPARSITY
   // END_FOR_HES_SPARSITY
}

Base
****
is the type corresponding to *parameter_x*
and to this atomic function.

InternalSparsity
****************
is the internal type used to represent sparsity; i.e.,
``sparse::pack_setvec`` or ``sparse::list_setvec`` .

atom_index
**********
is the index, in local::atomic_index, corresponding to this atomic function.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

parameter_x
***********
value of the parameter arguments to the atomic function
(other arguments have the value nan).

type_x
******
type for each component of x (not used by atomic_two interface).

x_index
*******
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.
We use *m_x* to denote the maximum value w.r.t *i* of
*x_index* [ *i* ] .

y_index
*******
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.
It should hold that *m_i* < ``y_index`` [ ``i`` ] for all *i* .

np1
***
This is the number of independent variables plus one;
i.e. size of *x* plus one.

num_var
*******
This is the total number of variables in the tape.

rev_jac_sparsity
****************
For i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the reverse Jacobian sparsity for the i-th result to this atomic function.
This shows which components of the result affect the function we are
computing the Hessian of.

for_sparsity
************
We have the conditions *np1* = *for_sparsity* . ``end`` ()
and *for_sparsity* . ``n_set`` () = *np1* + *num_var* .

Input Jacobian Sparsity
=======================
For *i* = 0, ..., *m_x* ,
the *np1* + *i* row of *for_sparsity* is the Jacobian sparsity
for the *i*-th variable. These values do not change.
Note that *i* =0 corresponds to a parameter and
the corresponding Jacobian sparsity is empty.

Input Hessian Sparsity
======================
For *i* =1, ..., *n* ,
the *i*-th row of *for_sparsity* is the Hessian sparsity
before including the function :math:`y = f(x)`.

Output Jacobian Sparsity
========================
For *i* =0, ..., *y_index* . ``size`` () ,
row *np1* + *y_index* [ *i* ]
of *for_sparsity* is the Jacobian sparsity
for the variable with index *y_index* [ *i* ] .

Output Hessian Sparsity
=======================
For *i* =1, ..., *n* ,
the *i*-th row of *for_sparsity* is the Hessian sparsity
after including the function :math:`y = f(x)`.

{xrst_end atomic_for_hes_sparsity_callback}
*/
// BEGIN_FOR_HES_SPARSITY
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_for_hes_sparsity(
   size_t                       atom_index        ,
   size_t                       call_id           ,
   const vector<Base>&          parameter_x       ,
   const vector<ad_type_enum>&  type_x            ,
   const vector<size_t>&        x_index           ,
   const vector<size_t>&        y_index           ,
   size_t                       np1               ,
   size_t                       num_var           ,
   const InternalSparsity&      rev_jac_sparsity  ,
   InternalSparsity&            for_sparsity      )
// END_FOR_HES_SPARSITY
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + num_var );

   bool         set_null = false;
   size_t       type     = 0;          // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
   //
   // ident_zero_x
   vector<bool> ident_zero_x;
   if( type == 4 )
   {  size_t n = x_index.size();
      ident_zero_x.resize(n);
      for(size_t j = 0; j < n; ++j)
      {  if( type_x[j] > constant_enum )
            ident_zero_x[j] = false;
         else
            ident_zero_x[j] = IdenticalZero( parameter_x[j] );
      }
   }
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if( ok )
   {
      if( type == 2 )
      {  atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
         afun->set_old(call_id);
         ok = afun->for_sparse_hes(
            parameter_x,
            x_index,
            y_index,
            np1,
            num_var,
            rev_jac_sparsity,
            for_sparsity
         );
      }
      else if( type == 3 )
      {  atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
         ok = afun->for_hes_sparsity(
            parameter_x,
            type_x,
            x_index,
            y_index,
            np1,
            num_var,
            rev_jac_sparsity,
            for_sparsity
         );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<RecBase>* afun =
            reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
         ok = afun->for_hes_sparsity(
            call_id,
            ident_zero_x,
            x_index,
            y_index,
            np1,
            num_var,
            rev_jac_sparsity,
            for_sparsity
         );
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string name;
      local::atomic_index<RecBase>(
         set_null, atom_index, type, &name, v_ptr
      );
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic hes_sparsity returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  atomic_base<RecBase>* afun =
         reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
      afun->set_old(call_id);
      afun->for_sparse_hes(
         parameter_x,
         x_index,
         y_index,
         np1,
         num_var,
         rev_jac_sparsity,
         for_sparsity
      );
   }
   else if( type == 3 )
   {  atomic_three<RecBase>* afun =
         reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
      afun->for_hes_sparsity(
         parameter_x,
         type_x,
         x_index,
         y_index,
         np1,
         num_var,
         rev_jac_sparsity,
         for_sparsity
      );
   }
   else
   {  atomic_four<RecBase>* afun =
         reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
      afun->for_hes_sparsity(
         call_id,
         ident_zero_x,
         x_index,
         y_index,
         np1,
         num_var,
         rev_jac_sparsity,
         for_sparsity
      );
   }
# endif
}
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_rev_hes_sparsity_callback dev}

Reverse Hessian Sparsity Callback to Atomic Functions
#####################################################

Prototype
*********
{xrst_literal
   // BEGIN_REV_HES_SPARSITY
   // END_REV_HES_SPARSITY
}

Base
****
is the type corresponding to parameter_x
and to this atomic function.

InternalSparsity
****************
is the internal type used to represent sparsity; i.e.,
sparse::pack_setvec or sparse::list_setvec.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

parameter_x
***********
value of the parameter arguments to the atomic function
(other arguments have the value nan).

type_x
******
type for each component of x (not used by atomic_two interface).

x_index
*******
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.

y_index
*******
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.

for_jac_sparsity
****************
For j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the forward Jacobian sparsity for the j-th argument to this atomic function.

rev_jac_flag
************
On input, for i = 0, ... , m-1, rev_jac_flag[ y_index[i] ] is true
if the function (we are computing the sparsity for)
depends on the variable y_index[i].
Upon return, for j = 0, ..., n-1, rev_jac_flag[ x_index[j] ] has been set to
true any of the y_index variables are flagged depend on x_index[j].
Otherwise, rev_jac_flag[ x_index[j] ] is not modified.

rev_hes_sparsity
****************
This is the sparsity pattern for the Hessian.
On input, for i = 0, ... , m-1, row y_index[i] is the reverse Hessian sparsity
with one of the partials with respect to y_index[i].
Upon return, for j = 0, ..., n-1, the row x_index[j] has been
modified to include components that have a non-zero hessian through
the atomic function with one of the partials w.r.t. x_index[j].

{xrst_end atomic_rev_hes_sparsity_callback}
*/
// BEGIN_REV_HES_SPARSITY
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_rev_hes_sparsity(
   size_t                       atom_index        ,
   size_t                       call_id           ,
   const vector<Base>&          parameter_x       ,
   const vector<ad_type_enum>&  type_x            ,
   const vector<size_t>&        x_index           ,
   const vector<size_t>&        y_index           ,
   const InternalSparsity&      for_jac_sparsity  ,
   bool*                        rev_jac_flag      ,
   InternalSparsity&            rev_hes_sparsity  )
// END_REV_HES_SPARSITY
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   bool         set_null = false;
   size_t       type     = 0;          // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
   //
   // ident_zero_x
   vector<bool> ident_zero_x;
   if( type == 4 )
   {  size_t n = x_index.size();
      ident_zero_x.resize(n);
      for(size_t j = 0; j < n; ++j)
      {  if( type_x[j] > constant_enum )
            ident_zero_x[j] = false;
         else
            ident_zero_x[j] = IdenticalZero( parameter_x[j] );
      }
   }
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if( ok )
   {
      if( type == 2 )
      {  atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
         afun->set_old(call_id);
         ok = afun->rev_sparse_hes(
            parameter_x,
            x_index,
            y_index,
            for_jac_sparsity,
            rev_jac_flag,
            rev_hes_sparsity
         );
      }
      else if( type == 3 )
      {  atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
         ok = afun->rev_hes_sparsity(
            parameter_x,
            type_x,
            x_index,
            y_index,
            for_jac_sparsity,
            rev_jac_flag,
            rev_hes_sparsity
         );
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<RecBase>* afun =
            reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
         ok = afun->rev_hes_sparsity(
            call_id,
            ident_zero_x,
            x_index,
            y_index,
            for_jac_sparsity,
            rev_jac_flag,
            rev_hes_sparsity
         );
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string name;
      local::atomic_index<RecBase>(
         set_null, atom_index, type, &name, v_ptr
      );
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic hes_sparsity returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  atomic_base<RecBase>* afun =
         reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
      afun->set_old(call_id);
      afun->rev_sparse_hes(
         parameter_x,
         x_index,
         y_index,
         for_jac_sparsity,
         rev_jac_flag,
         rev_hes_sparsity
      );
   }
   else if( type == 3 )
   {  atomic_three<RecBase>* afun =
         reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
      afun->rev_hes_sparsity(
         parameter_x,
         type_x,
         x_index,
         y_index,
         for_jac_sparsity,
         rev_jac_flag,
         rev_hes_sparsity
      );
   }
   else
   {  atomic_four<RecBase>* afun =
         reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
      afun->rev_hes_sparsity(
         call_id,
         ident_zero_x,
         x_index,
         y_index,
         for_jac_sparsity,
         rev_jac_flag,
         rev_hes_sparsity
      );
   }
# endif
}
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_rev_depend_callback dev}

Reverse Dependency Callback to Atomic Functions
###############################################

Prototype
*********
{xrst_literal
   // BEGIN_REV_DEPEND
   // END_REV_DEPEND
}

atom_index
**********
is the index, in local::atomic_index, corresponding to this atomic function.

call_id
*******
see the atomic_four :ref:`atomic_four_call@call_id` and
the atomic_one :ref:`atomic_one@id` .

parameter_x
***********
is the value of the parameters in the corresponding atomic function call.

type_x
******
is the type for each x component in the corresponding atomic function call.

depend_x
********
which components of x affect values we are interested in.
This is the only output for this routine.

depend_y
********
which components of y affect values we are interested in.

{xrst_end atomic_rev_depend_callback}
*/
// BEGIN_REV_DEPEND
template <class Base, class RecBase>
void call_atomic_rev_depend(
   size_t                      atom_index   ,
   size_t                      call_id      ,
   const vector<Base>&         parameter_x  ,
   const vector<ad_type_enum>& type_x       ,
   vector<bool>&               depend_x     ,
   const vector<bool>&         depend_y     )
// END_REV_DEPEND
{  CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
   bool         set_null = false;
   size_t       type     = 0;          // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
   //
   // ident_zero_x
   vector<bool> ident_zero_x;
   if( type == 4 )
   {  size_t n = parameter_x.size();
      ident_zero_x.resize(n);
      for(size_t j = 0; j < n; ++j)
      {  if( type_x[j] > constant_enum )
            ident_zero_x[j] = false;
         else
            ident_zero_x[j] = IdenticalZero( parameter_x[j] );
      }
   }
# ifndef NDEBUG
   bool ok = v_ptr != nullptr;
   if( ok )
   {
      if( type == 2 )
      {  atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
         afun->set_old(call_id);
         vector<ad_type_enum> empty;
         ok = afun->rev_depend(parameter_x, type_x, depend_x, depend_y);
      }
      else if( type == 3 )
      {  atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
         ok = afun->rev_depend(parameter_x, type_x, depend_x, depend_y);
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( type == 4 );
         atomic_four<RecBase>* afun =
            reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
         // rev_depend does not have a wrapper that drops back to
         // deprecated version of this function.
         ok = afun->rev_depend(call_id, ident_zero_x, depend_x, depend_y);
         if( ! ok ) ok = afun->rev_depend(call_id, depend_x, depend_y);
      }
   }
   if( ! ok )
   {  // now take the extra time to copy the name
      std::string name;
      local::atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
      std::string msg = name;
      if( v_ptr == nullptr )
         msg += ": this atomic function has been deleted";
      else
         msg += ": atomic rev_depend returned false";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# else
   if( type == 2 )
   {  atomic_base<RecBase>* afun =
         reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
      vector<ad_type_enum> empty;
      afun->set_old(call_id);
      afun->rev_depend(parameter_x, type_x, depend_x, depend_y);
   }
   else if( type == 3 )
   {  atomic_three<RecBase>* afun =
         reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
      afun->rev_depend(parameter_x, type_x, depend_x, depend_y);
   }
   else
   {  atomic_four<RecBase>* afun =
         reinterpret_cast< atomic_four<RecBase>* >(v_ptr);
      // rev_depend does not have a wrapper that drops back to
      // deprecated version of this function.
      bool ok = afun->rev_depend(call_id, ident_zero_x, depend_x, depend_y);
      if( ! ok ) afun->rev_depend(call_id, depend_x, depend_y);
   }
# endif
}


} } } // END_CPAPD_LOCAL_SWEEP_NAMESPACE
# endif
