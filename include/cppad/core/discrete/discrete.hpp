# ifndef CPPAD_CORE_DISCRETE_DISCRETE_HPP
# define CPPAD_CORE_DISCRETE_DISCRETE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <vector>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_code_dyn.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*
 ------------------------------------------------------------------------------
{xrst_begin discrete_create dev}
Create a Discrete AD Function
#############################

Syntax
******
| ``CPPAD_DISCRETE_FUNCTION`` ( *Base* , *name* )
| ``name`` ( *ax* , *ay* )

Base
****
is the base type for the discrete function.

name
****
is the name of the user defined function that corresponding to this operation.

ax
**
Is a ``AD`` < *Base* > corresponding to the argument for the function.

ay
**
Is a ``AD`` < *Base* > corresponding to the result for the function.

fun
***
The local object ``fun`` is a member of the ``discrete`` class.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
# define CPPAD_DISCRETE_FUNCTION(Base, name)            \
inline CppAD::AD<Base> name (const CppAD::AD<Base>& ax) \
{    static CppAD::discrete<Base> fun(#name, name);     \
     return fun.ad(ax);                                 \
}                                                       \
CppAD::AD<Base> this_variable_name_used_to_initialize_discrete_ ##name =  \
name( CppAD::AD<Base>(0.0) );
//
# define CppADCreateDiscrete CPPAD_DISCRETE_FUNCTION
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_create}
-----------------------------------------------------------------------------
{xrst_begin discrete_class dev}

Declare discrete Class and Member Data
######################################

parallel_ad
***********
is a friend of this class so it can call List to initialize
its static data.

Fun
***
is the type for the user routine that computes *Base* function values.

name\_
******
name of this user defined discrete function.

f\_
***
user routine that computes *Base* function values.

index\_
*******
index of this object in :ref:`discrete_list-name` for this *Base* .

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
template <class Base>
class discrete {
private:
   template <class Type> friend void parallel_ad(void);
   typedef Base (*Fun) (const Base& x);
   const std::string    name_;
   const Fun            f_;
   const size_t         index_;
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_class}
------------------------------------------------------------------------------
{xrst_begin discrete_list dev}
List of all objects in the discrete class
#########################################

Syntax
******
*list* = ``discrete`` < *Base* >:: ``List`` ()

Base
****
Is the :ref:`discrete_create@Base`
type for this list of discrete functions.

list
****
is a reference to the list of all the
``discrete`` object currently defined.

std::vector
===========
We use ``std::vector`` instead of ``CppAD::vector``
so it does not appear that there is a :ref:`memory_leak-name`
this list is not destroyed before
:ref:`thread_alloc::free_all<ta_free_all-name>` is called by testing routines.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
private:
   static std::vector<discrete *>& List(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static std::vector<discrete *> list;
      return list;
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_list}
 ------------------------------------------------------------------------------
{xrst_begin discrete_list_size dev}
Size of the Discrete Function List
##################################

Syntax
******
*size* = ``discrete`` < *Base* >:: ``list_size`` ()

Base
****
Is the :ref:`discrete_create@Base`
type for this list of discrete functions.

size
****
is the number of discrete functions for this *Base* type.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
public:
   static size_t list_size(void)
   {  return List().size(); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_list_size}
 ------------------------------------------------------------------------------
{xrst_begin discrete_ctor dev}
Constructor Called by each Use of CPPAD_DISCRETE_FUNCTION
#########################################################

Syntax
******
``discrete`` < *Base* > *fun* ( *name* , *f* )

name
****
is the name of this function.

f
*
user routine that implements this function for Base class.

fun
***
is the ``discrete`` object created by this call to the constructor.

name\_
======
is set equal to *name* .

f\_
===
is set equal to *f* .

index\_
=======
This object is put at the end of :ref:`discrete_list-name` and ``index_``
is set to the index of this object in the discrete list.

Parallel
********
This constructor cannot be used in parallel mode because it changes
the static object returned by :ref:`discrete_list-name` .

{xrst_end discrete_ctor}
*/
public:
   discrete(const std::string& name, Fun f) :
   name_(name), f_(f) , index_( List().size() )
   {  std::string msg = "discrete: first call to the discrete function ";
      msg  += name;
      msg  += " is in parallel mode.";
      CPPAD_ASSERT_KNOWN(
         ! thread_alloc::in_parallel() ,
         msg.c_str()
      );
      List().push_back(this);
   }
/*
 ------------------------------------------------------------------------------
{xrst_begin discrete_ad dev}
Implement AD Version of a Discrete Function
###########################################

Syntax
******
*ay* = *fun* . ``ad`` ( *ax* )

ax
**
is the argument for the AD version of this function.

ay
**
is the return value for the AD version of this function.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
   AD<Base> ad(const AD<Base> &ax) const
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_ad}
*/
   {
      CPPAD_ASSERT_KNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= index_,
         "discrete: cppad_tape_addr_type maximum not large enough"
      );
      //
      AD<Base> ay;
      ay.value_ = f_(ax.value_);
      //
      // check if there is a recording in progress
      local::ADTape<Base>* tape = AD<Base>::tape_ptr();
      if( tape == nullptr )
         return ay;
      //
      // check if argument is a constant parameter
      if( ax.tape_id_ != tape->id_ )
         return ay;
      //
      if( ax.ad_type_ == dynamic_enum )
      {
         // tape dynamic paramter operation
         ay.taddr_   = tape->Rec_.put_dyn_par(
            ay.value_, local::dis_dyn, addr_t(index_), ax.taddr_
         );
         ay.tape_id_  = ax.tape_id_;
         ay.ad_type_  = dynamic_enum;

         // make result a dynamic parameter
         ay.tape_id_    = tape->id_;
         ay.ad_type_    = dynamic_enum;

         CPPAD_ASSERT_UNKNOWN( Dynamic(ay) );
      }
      else if( ax.ad_type_ == variable_enum )
      {
         CPPAD_ASSERT_UNKNOWN( local::NumRes(local::DisOp) == 1 );
         CPPAD_ASSERT_UNKNOWN( local::NumArg(local::DisOp) == 2 );

         // put operand addresses in the tape
         CPPAD_ASSERT_KNOWN(
            size_t( std::numeric_limits<addr_t>::max() ) >= index_,
            "discrete: cppad_tape_addr_type maximum not large enough"
         );
         tape->Rec_.PutArg(addr_t(index_), ax.taddr_);
         // put operator in the tape
         ay.taddr_ = tape->Rec_.PutOp(local::DisOp);
         // make result a variable
         ay.tape_id_    = tape->id_;
         ay.ad_type_    = variable_enum;

         CPPAD_ASSERT_UNKNOWN( Variable(ay) );
      }
      else
      {  // other types not yet being used and should have this tape id
         CPPAD_ASSERT_UNKNOWN(false);
      }
      return ay;
   }
/*
 ------------------------------------------------------------------------------
{xrst_begin discrete_index dev}
{xrst_spell
   msg
   str
}

Index That Identifies a Discrete Function
#########################################
{xrst_code hpp} */
static size_t index(const std::string& name)
{  size_t i = List().size();
   while(i--)
   {  if( List()[i]->name_ == name )
         return i;
   }
   std::string msg = "There is no discrete function named " + name;
   CPPAD_ASSERT_KNOWN(false, msg.c_str());
   return List().size();
}
/* {xrst_code}

{xrst_end discrete_index}
*/
/*
------------------------------------------------------------------------------
{xrst_begin discrete_name dev}

Name Corresponding to a discrete Function
#########################################

Syntax
******
``discrete`` < *Base* >:: ``name`` ( *index* )

Base
****
Is the :ref:`discrete_create@Base`
type for this list of discrete functions.

index
*****
Is the index, in the list, for this discrete function.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
   static const std::string& name(size_t index)
   {  return List()[index]->name_; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_name}
------------------------------------------------------------------------------
{xrst_begin discrete_eval dev}
Link From Forward Mode Sweep to Users Routine
#############################################

Syntax
******
*y* = ``discrete`` < *Base* >:: ``eval`` ( *index* , *x* )

Base
****
Is the :ref:`discrete_create@Base`
type for this list of discrete functions.

index
*****
index for this function in :ref:`discrete_list-name` .

x
*
argument at which to evaluate *Base* version of this function.

y
*
result for the *Base* version of this function.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
   static Base eval(size_t index, const Base& x)
   {  CPPAD_ASSERT_UNKNOWN(index < List().size() );
      return List()[index]->f_(x);
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_eval}
------------------------------------------------------------------------------
{xrst_begin discrete_ad_eval dev}
Link From Forward Mode Sweep to AD Version of Discrete Function
###############################################################

Syntax
******
*ay* = ``discrete`` < *Base* >:: ``eval`` ( *index* , *ax* )

Base
****
Is the :ref:`discrete_create@Base`
type for this list of discrete functions.

index
*****
index for this function in :ref:`discrete_list-name` .

ax
**
argument at which to evaluate ``AD`` < *Base* > version of this function.

ay
**
result for the ``AD`` < *Base* > version of this function.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
   static AD<Base> ad_eval(size_t index, const AD<Base>& ax)
   {  CPPAD_ASSERT_UNKNOWN(index < List().size() );
      return List()[index]->ad(ax);
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end discrete_ad_eval}
*/
};

} // END_CPPAD_NAMESPACE
# endif
