# ifndef CPPAD_LOCAL_SWEEP_CALL_ATOMIC_HPP
# define CPPAD_LOCAL_SWEEP_CALL_ATOMIC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/local/atomic_index.hpp>
# include <cppad/core/atomic/atomic_two.hpp>
# include <cppad/core/atomic/atomic_three.hpp>

// BEGIN_CPAPD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file call_atomic.hpp
Callbacks to atomic functions corresponding to atomic_index.
*/
// ----------------------------------------------------------------------------
/*!
Forward mode callback to atomic functions.

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param var_x [in]
if size not zero, which components of x are variables

\param var_y [out]
if size not zero, which components of y are variables

\param taylor_x [in]
Taylor coefficients corresponding to x.

\param taylor_y [out]
Taylor coefficient corresponding to y.
*/
template <class Base, class RecBase>
void call_atomic_forward(
    size_t                       order_low  ,
    size_t                       order_up   ,
    size_t                       atom_index ,
    size_t                       atom_old   ,
    const vector<bool>&          var_x      ,
    vector<bool>&                var_y      ,
    const vector<Base>&          taylor_x   ,
    vector<Base>&                taylor_y   )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type;
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr;
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
    CPPAD_ASSERT_UNKNOWN( type == 2 );
    //
# ifndef NDEBUG
    bool ok;
    atomic_base<RecBase>* afun;
    afun = reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
    afun->set_old(atom_old);
    ok = afun->forward(
        order_low, order_up, var_x, var_y, taylor_x, taylor_y
    );
    if( ! ok )
    {   // now take the extra time to copy the name
        std::string name;
        local::atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
        std::string msg = name + ": atomic forward returned false";
        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
    }
# else
    atomic_base<RecBase>* afun;
    afun = reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
    afun->set_old(atom_old);
    afun->forward(
        order_low, order_up, var_x, var_y, taylor_x, taylor_y
    );
# endif
}
// ----------------------------------------------------------------------------
/*!
Reverse mode callback to atomic functions.

\param order_up [in]
highest order for this reverse mode calculation.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param taylor_x [in]
Taylor coefficients corresponding to x.

\param taylor_y [in]
Taylor coefficient corresponding to y.

\param patrial_x [out]
Partials w.r.t the x Taylor coefficients.

\param taylor_y [in]
Partials w.r.t the y Taylor coefficients.
*/
template <class Base, class RecBase>
void call_atomic_reverse(
    size_t                       order_up   ,
    size_t                       atom_index ,
    size_t                       atom_old   ,
    const vector<Base>&          taylor_x   ,
    const vector<Base>&          taylor_y   ,
    vector<Base>&                partial_x  ,
    const vector<Base>&          partial_y  )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type;
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr;
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
    CPPAD_ASSERT_UNKNOWN( type == 2 );
    //
# ifndef NDEBUG
    bool ok;
    atomic_base<RecBase>* afun;
    afun = reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
    afun->set_old(atom_old);
    ok = afun->reverse(
        order_up, taylor_x, taylor_y, partial_x, partial_y
    );
    if( ! ok )
    {   // now take the extra time to copy the name
        std::string name;
        local::atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
        std::string msg = name + ": atomic reverse returned false";
        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
    }
# else
    atomic_base<RecBase>* afun;
    afun = reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
    afun->set_old(atom_old);
    afun->reverse(
        order_up, taylor_x, taylor_y, partial_x, partial_y
    );
# endif
}


} } } // END_CPAPD_LOCAL_SWEEP_NAMESPACE
# endif
