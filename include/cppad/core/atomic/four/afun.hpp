# ifndef CPPAD_CORE_ATOMIC_FOUR_AFUN_HPP
# define CPPAD_CORE_ATOMIC_FOUR_AFUN_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_four_afun$$

$spell
    sq
    mul
    afun
    const
    CppAD
    mat_mul.cpp
    std
    cppad
$$

$section Calling an Atomic Function$$

$head Syntax$$
$icode%afun%(%ax%, %ay%)
%$$
$icode%ay% = %afun%(%call_id%, %ax%, %ay%)
%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Purpose$$
Given $icode ax$$, this call computes the corresponding value of $icode ay$$.
If $codei%AD<%Base%>%$$ operations are being recorded,
it enters the computation as an atomic operation in the recording;
see $cref/start recording/Independent/Start Recording/$$.

$head Base$$
This is the $icode Base$$ type of the elements of $icode ax$$ and $icode ay$$
in the call to the $icode afun$$ atomic operation.
To be specific, the elements of $icode ax$$ and $icode ay$$ have type
$codei%AD%<%Base%>%$$.

$head ADVector$$
The type $icode ADVector$$ must be a
$cref/simple vector class/SimpleVector/$$ with elements of type
$codei%AD<%Base%>%$$.

$head afun$$
is a $cref/atomic_user/atomic_four_ctor/atomic_user/$$ object
and this $icode afun$$ function call is implemented by the
$cref/atomic_four/atomic_four_ctor/atomic_four/$$ class.

$head ax$$
The size of this vector determines$icode n$$.
It specifies vector $latex x \in \B{R}^n$$
at which an $codei%AD<%Base%>%$$ version of
$latex y = g(x)$$ is to be evaluated; see
$cref/Base/atomic_four_ctor/atomic_four/Base/$$.

$head ay$$
The size of this vector determines $icode m$$.
The input values of its elements
are not specified (must not matter).
Upon return, it is an $codei%AD<%Base%>%$$ version of
$latex y = g(x)$$.

$head call_id$$
This optional argument has default value zero.
It can be used to specify additional information about this call to
$icode afun$$. For example, it could specify the index in vector of structures
in the $icode afun$$ object where the actual information is placed.

$subhead Restriction$$
The value of $icode call_id$$ must be less than or equal
$codei%
    std::numeric_limits<%cppad_tape_id_type%>::max()
%$$
see $cref/cppad_tape_id_type/cmake/cppad_tape_id_type/$$.

$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class Base> template <class ADVector>
void atomic_four<Base>::operator()(
    size_t           call_id ,
    const ADVector&  ax      ,
    ADVector&        ay      )
// END_PROTOTYPE
{
    size_t n = ax.size();
    size_t m = ay.size();
# ifndef NDEBUG
    bool ok = true;
    std::string msg = "atomic_four: call " + atomic_name() + " ";
    if( (n == 0) | (m == 0) )
    {   msg += "ax.size() or ay.size() is zero";
        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
    }
# endif
    size_t thread = thread_alloc::thread_num();
    allocate_work(thread);
    vector<Base>& taylor_x        = work_[thread]->taylor_x;
    vector<Base>& taylor_y        = work_[thread]->taylor_y;
    vector<ad_type_enum>& type_x  = work_[thread]->type_x;
    vector<ad_type_enum>& type_y  = work_[thread]->type_y;
    //
    type_x.resize(n);
    taylor_x.resize(n);
    //
    type_y.resize(m);
    taylor_y.resize(m);
    //
    // Determine tape corresponding to variables in ax
    tape_id_t            tape_id  = 0;
    local::ADTape<Base>* tape     = nullptr;
    for(size_t j = 0; j < n; j++)
    {   taylor_x[j]  = ax[j].value_;
        if( Constant( ax[j] ) )
            type_x[j] = constant_enum;
        else
        {   type_x[j] = ax[j].ad_type_;
            if( tape_id == 0 )
            {   tape    = ax[j].tape_this();
                tape_id = ax[j].tape_id_;
                CPPAD_ASSERT_UNKNOWN( tape != nullptr );
            }
# ifndef NDEBUG
            if( Dynamic( ax[j] ) )
            {    CPPAD_ASSERT_UNKNOWN( type_x[j] == dynamic_enum );
            }
            else
            {   CPPAD_ASSERT_UNKNOWN( Variable( ax[j] ) );
                CPPAD_ASSERT_UNKNOWN( type_x[j] == variable_enum );
            }
            if( tape_id != ax[j].tape_id_ )
            {   msg += atomic_name() +
                ": ax contains non-constant values from different threads.";
                CPPAD_ASSERT_KNOWN(false, msg.c_str());
            }
# endif
        }
    }
    // Use zero order forward mode to compute all the components of y
    size_t need_y    = size_t(variable_enum) + 1;
    size_t order_begin = 0;
    size_t order_end  = 0;
    CPPAD_ASSERT_UNKNOWN( need_y > size_t(variable_enum) );
# ifdef NDEBUG
    forward(taylor_x, type_x, need_y, order_begin, order_end, taylor_x, taylor_y);
    for(size_t j = 0; j < n; ++j)
        if( type_x[j] == variable_enum )
            taylor_x[j] = CppAD::numeric_limits<Base>::quiet_NaN();
    for_type(taylor_x, type_x, type_y);
# else
    ok &= forward(
        taylor_x, type_x, need_y, order_begin, order_end, taylor_x, taylor_y
    );
    for(size_t j = 0; j < n; ++j)
        if( type_x[j] == variable_enum )
            taylor_x[j] = CppAD::numeric_limits<Base>::quiet_NaN();
    ok &= for_type(taylor_x, type_x, type_y);
    if( ! ok )
    {   msg += atomic_name() + ": ok is false for "
            "type or zero order forward mode calculation.";
        CPPAD_ASSERT_KNOWN(false, msg.c_str());
    }
# endif
    bool record_dynamic = false;
    bool record_variable = false;
    //
    // set ay to be vector of constant parameters with correct value
    for(size_t i = 0; i < m; i++)
    {   // pass back values
        ay[i].value_ = taylor_y[i];

        // initialize entire vector as constants
        ay[i].tape_id_ = 0;
        ay[i].taddr_   = 0;

        // we need to record this operation if
        // any of the elemnts of ay are dynamics or variables,
        record_dynamic  |= type_y[i] == dynamic_enum;
        record_variable |= type_y[i] == variable_enum;
    }
# ifndef NDEBUG
    if( (record_dynamic || record_variable) && tape == nullptr )
    {   msg +=
        "all elements of x are constants but y contains a non-constant";
        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
    }
# endif
    if( record_dynamic)
    {   tape->Rec_.put_dyn_atomic(
            tape_id, index_, call_id, type_x, type_y, ax, ay
        );
    }
    // case where result contains a variable
    if( record_variable )
    {   tape->Rec_.put_var_atomic(
            tape_id, index_, call_id, type_x, type_y, ax, ay
        );
    }
# ifndef NDEBUG
    for(size_t i = 0; i < m; ++i) switch( type_y[i] )
    {   //
        case constant_enum:
        CPPAD_ASSERT_UNKNOWN( Constant( ay[i] ) );
        break;
        //
        case dynamic_enum:
        CPPAD_ASSERT_UNKNOWN( Dynamic( ay[i] ) );
        break;
        //
        case variable_enum:
        CPPAD_ASSERT_UNKNOWN( Variable( ay[i] ) );
        break;
        //
        default:
        CPPAD_ASSERT_KNOWN( false,
            "atomic_four: for_type: type_y[i]: is not a valid type"
        );
        break;
    }
# endif
    return;
}
template <class Base> template <class ADVector>
void atomic_four<Base>::operator()(
    const ADVector&  ax      ,
    ADVector&        ay      )
{   size_t call_id = 0;
    (*this)(call_id, ax, ay);
}

} // END_CPPAD_NAMESPACE
# endif
