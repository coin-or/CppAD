# ifndef CPPAD_CORE_CHKPOINT_TWO_CTOR_HPP
# define CPPAD_CORE_CHKPOINT_TWO_CTOR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin chkpoint_two_ctor$$
$spell
    chkpoint
    chk
    bool
$$

$section Checkpoint Function Constructor$$

$head Syntax$$
$codei%chkpoint_two<%Base%> %chk_fun%(
    %fun%, %name%, %internal_bool%, %use_base2ad%, %use_in_parallel%
)%$$

$head Prototype$$
$srcfile%include/cppad/core/chkpoint_two/ctor.hpp%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Parallel$$
This constructor, and its corresponding destructor, must not be called in
$cref/parallel/ta_in_parallel/$$ mode.
The object $icode chk_fun$$ should not be destructed for as long as there is
an $codei%ADFun<%Base%>%$$ object the has $icode chk_fun$$ in its recording.

$head Base$$
The type $icode Base$$ specifies the base type for AD operations.

$head fun$$
This specifies the function $latex g(x)$$.
Note that $icode fun$$ may or may not have been
$cref/optimized/optimize/$$ before calling the constructor.
This will determine if the internal representation for $icode g(x)$$
is optimized.

$head name$$
is the name used for reporting errors using this checkpoint function.

$head internal_bool$$
If true, sparsity calculations are done with sets represented
by vectors of boolean values.
Otherwise, vectors of sets are used for sparsity patterns.

$head use_base2ad$$
If this is true, $icode chk_fun$$ can be used during the recording
of $codei%ADFun<%Base%>%$$ objects that get converted to
$codei%ADFun< AD<%Base%> >%$$ objects using $cref base2ad$$.
This requires some extra memory and extra computation during the constructor.

$head use_in_parallel$$
If this is true, $icode chk_fun$$ can be used
$cref/in_parallel/ta_parallel_setup/in_parallel/$$.
This requires some extra memory for a constant copy of the $icode fun$$
information and a separate copy (that changes) for each thread.

$head chk_fun$$
This is a checkpoint function representation of $latex g(x)$$
that can be used during the recording of $codei%AD<%Base%>%$$ operations.

$end
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file chkpoint_two/ctor.hpp
Constructor for chkpoint_two class.
*/

/*!
Constructor

\tparam Base
base class for recording AD<Base> operations using this checkpoint object.

\param fun
is the function g(x) corresponding to this checkpoint object.

\param name
is the name used for error reporting.

\param internal_bool
should sparisity calculations be done using bools (or sets).

\param use_base2ad
will this checkpoint function be used with base2ad.

\param use_in_parallel
will this checkpoint function be used in parallel mode.
*/

// BEGIN_PROTOTYPE
template <class Base>
chkpoint_two<Base>::chkpoint_two(
        const ADFun<Base>& fun    ,
        const std::string& name   ,
        bool  internal_bool       ,
        bool  use_base2ad         ,
        bool  use_in_parallel     )
// END_PROTOTYPE
:
atomic_three<Base>(name)              ,
internal_bool_( internal_bool )       ,
use_base2ad_ ( use_base2ad )          ,
use_in_parallel_ ( use_in_parallel )
{   CPPAD_ASSERT_KNOWN(
        ! thread_alloc::in_parallel() ,
        "chkpoint_two: constructor cannot be called in parallel mode."
    );
    // initialize member pointers as null;
    for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
        member_[thread] = CPPAD_NULL;
    //
    // g_
    g_ = fun;
    //
    // ag_
    if( use_base2ad )
        ag_ = g_.base2ad();
    //
    // jac_sparsity_
    size_t n = g_.Domain();
    size_t m = g_.Range();
    sparse_rc< vector<size_t> > pattern_in;
    bool transpose     = false;
    bool dependency    = true;
    if( n <= m )
    {   // use forward mode
        pattern_in.resize(n, n, n);
        for(size_t k = 0; k < n; ++k)
            pattern_in.set(k, k, k);
        g_.for_jac_sparsity(
            pattern_in,
            transpose,
            dependency,
            internal_bool,
            jac_sparsity_
        );
        if( internal_bool )
            g_.size_forward_bool(0);
        else
            g_.size_forward_set(0);
    }
    else
    {   // use reverse mode
        pattern_in.resize(m, m, m);
        for(size_t k = 0; k < m; ++k)
            pattern_in.set(k, k, k);
        g_.rev_jac_sparsity(
            pattern_in,
            transpose,
            dependency,
            internal_bool,
            jac_sparsity_
        );
    }
    CPPAD_ASSERT_UNKNOWN( g_.size_forward_bool() == 0 );
    CPPAD_ASSERT_UNKNOWN( g_.size_forward_set()  == 0 );
}
/// destructor
template <class Base>
chkpoint_two<Base>::~chkpoint_two(void)
{
# ifndef NDEBUG
    if( thread_alloc::in_parallel() )
    {   std::string msg = atomic_three<Base>::afun_name();
        msg += ": chkpoint_two destructor called in parallel mode.";
        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
    }
# endif
    for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
        free_member(thread);
    }
} // END_CPPAD_NAMESPACE
# endif
