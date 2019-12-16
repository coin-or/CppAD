# ifndef CPPAD_CORE_DISCRETE_DISCRETE_HPP
# define CPPAD_CORE_DISCRETE_DISCRETE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <vector>
# include <cppad/core/cppad_assert.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file discrete.hpp
user define discrete functions
*/

/*!
\def CPPAD_DISCRETE_FUNCTION(Base, name)
Defines the function <code>name(ax, ay)</code>
where ax and ay are vectors with <code>AD<Base></code> elements.

\par Base
is the base type for the discrete function.

\par name
is the name of the user defined function that corresponding to this operation.
*/

# define CPPAD_DISCRETE_FUNCTION(Base, name)            \
inline CppAD::AD<Base> name (const CppAD::AD<Base>& ax) \
{                                                       \
     static CppAD::discrete<Base> fun(#name, name);     \
                                                        \
     return fun.ad(ax);                                 \
}
# define CppADCreateDiscrete CPPAD_DISCRETE_FUNCTION


/*
Class that acutally implemnets the <code>ay = name(ax)</code> call.

A new discrete function is generated for ech time the user invokes
the CPPAD_DISCRETE_FUNCTION macro; see static object in that macro.
*/
template <class Base>
class discrete {
    /// parallel_ad needs to call List to initialize static
    template <class Type>
    friend void parallel_ad(void);

    /// type for the user routine that computes function values
    typedef Base (*F) (const Base& x);
private:
    /// name of this user defined function
    const std::string name_;
    /// user's implementation of the function for Base operations
    const F              f_;
    /// index of this objec in the vector of all objects for this class
    const size_t     index_;

    /*!
    List of all objects in this class.

    If we use CppAD::vector for this vector, it will appear that
    there is a memory leak because this list is not distroyed before
    thread_alloc::free_available(thread) is called by the testing routines.
    */
    static std::vector<discrete *>& List(void)
    {   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
        static std::vector<discrete *> list;
        return list;
    }
public:
    /*!
    Constructor called for each invocation of CPPAD_DISCRETE_FUNCTION.

    Put this object in the list of all objects for this class and set
    the constant private data name_, f_, and index_.

    \param Name
    is the user's name for this discrete function.

    \param f
    user routine that implements this function for Base class.

    \par
    This constructor can ont be used in parallel mode because it changes
    the static object List.
    */
    discrete(const char* Name, F f) :
    name_(Name)
    , f_(f)
    , index_( List().size() )
    {
        CPPAD_ASSERT_KNOWN(
            ! thread_alloc::in_parallel() ,
            "discrete: First call the function *Name is in parallel mode."
        );
        List().push_back(this);
    }

    /*!
    Implement the user call to <code>ay = name(ax)</code>.

    \param ax
    is the argument for this call.

    \return
    the return value is called ay above.
    */
    AD<Base> ad(const AD<Base> &ax) const
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
        if( tape == CPPAD_NULL )
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
        {   // other types not yet being used and should have this tape id
            CPPAD_ASSERT_UNKNOWN(false);
        }
        return ay;
    }

    /// Name corresponding to a discrete object
    static const char* name(size_t index)
    {   return List()[index]->name_.c_str(); }

    /*!
    Link from forward mode sweep to users routine

    \param index
    index for this function in the list of all discrete object

    \param x
    argument value at which to evaluate this function
    */
    static Base eval(size_t index, const Base& x)
    {
        CPPAD_ASSERT_UNKNOWN(index < List().size() );

        return List()[index]->f_(x);
    }
};

} // END_CPPAD_NAMESPACE
# endif
