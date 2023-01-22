# ifndef CPPAD_VAL_GRAPH_MAP_BASE_HPP
# define CPPAD_VAL_GRAPH_MAP_BASE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// -------------------------------------------------------------
# include "type.hpp"
/*
{xrst_begin val_graph_map_base dev}
{xrst_spell
   xam
}

The Map Base Class
##################

Syntax
******
::

   template <class Value>
   class derived_t : public map_base_t<Value> { ...


Purpose
*******
This the base class for defining functions corresponding to the
:ref:`map_op <val_map_op-name>` .
The pure virtual members must be defined by the derived class.

map_id
******
{xrst_literal
   // BEGIN_MAP_ID
   // END_MAP_ID
}
This returns the mapping identifier for a derived class object.

map_name
********
{xrst_literal
   // BEGIN_MAP_NAME
   // END_MAP_NAME
}
This returns a name for the function
corresponding to the derived class object.

map_base_ptr
************
{xrst_literal
   // BEGIN_MAP_BASE_PTR
   // END_MAP_BASE_PTR
}
This returns a pointer that can be used to access the
derived class object corresponding to the specified map_id .

forward
*******
{xrst_literal
   // BEGIN_FORWARD
   // END_FORWARD
}

call_id
=======
The *map_id* combined with the *call_id* yields the function
corresponding to a map operator usage.
For example, one mapping might be used for matrix multiplication
and the *call_id* might identify what the matrix dimensions are.

x
=
This is the argument vector for the function call.

y
=
This is the return vector for the function call.
The size of *y* is correct on input and should not change.
The input value of its elements are not specified.

rev_depend
**********
{xrst_literal
   // BEGIN_REV_DEPEND
   // END_REV_DEPEND
}
This routine performs reverse dependency analysis.

call_id
=======
The has the same meaning as above.

depend_y
========
This has size equal to the number of results for the function call.
It the *i*-th component of this vector is true (false),
the results of interest depend (do not depend)
on the value of y[i].

depend_x
========
This has size equal to the number of arguments for the function call.
For each argument index *j*,
if there is an *i* such that,
depend_y[i] is true, and y[i] depends on x[j],
then depend_x[j] must  be set to true.
The size of *depend_x* is correct on input and should not change.
Its element values on input are not specified.

clear
*****
This is used to free memory that holds information about the
derived member objects.
The destructor for all the derived member objects must be called
before clear is called.

{xrst_comment map_xam.cpp is included by map_op.hpp}
Example
*******
The file :ref:`map_xam.cpp <val_map_op_xam.cpp-name>`
is an example and test that uses this class.

{xrst_end val_graph_map_base}
*/
template <class Value>
class map_base_t {
private:
   inline static Vector<map_base_t*> map_base_vec_;
   inline static Vector<bool>        deleted_;
   const size_t                      map_id_;
public:
   // ctor
   map_base_t(void) :
   map_id_ ( map_base_vec_.size() )
   {  // 2DO: cannot be in parallel mode
      map_base_vec_.push_back( this );
      deleted_.push_back( false );
   }
   ~map_base_t(void)
   {  // 2DO: cannot be in parallel mode
      deleted_[map_id_] = true;
   }
   // BEGIN_MAP_ID
   size_t map_id(void)
   // END_MAP_ID
   {  return map_id_;
   }
   // BEGIN_MAP_NAME
   virtual std::string map_name(void) const = 0;
   // END_MAP_NAME
   //
   // BEGIN_MAP_BASE_PTR
   static map_base_t* map_base_ptr(size_t map_id)
   // END_MAP_BASE_PTR
   {  assert( ! deleted_[map_id] );
      return map_base_vec_[map_id];
   }
   // BEGIN_FORWARD
   virtual bool forward(
      size_t               call_id       ,
      const Vector<Value>& x             ,
      Vector<Value>&       y             ) const = 0;
   // END_FORWARD
   // BEGIN_REV_DEPEND
   virtual bool rev_depend(
      size_t              call_id        ,
      Vector<bool>&       depend_x       ,
      const Vector<bool>& depend_y       ) const = 0;
   // END_REV_DEPEND
   //
   // BEGIN_CLEAR
   static bool clear(void)
   // END_CLEAR
   {  for(size_t i = 0; i < deleted_.size(); ++i)
         if( ! deleted_[i] )
            return false;
      map_base_vec_.clear();
      deleted_.clear();
      return true;
   }
};

# endif
