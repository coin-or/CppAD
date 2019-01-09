# ifndef CPPAD_CORE_CHKPOINT_TWO_CHKPOINT_TWO_HPP
# define CPPAD_CORE_CHKPOINT_TWO_CHKPOINT_TWO_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file chkpoint_two.hpp
Second generation checkpoint functions.
*/

/*
$begin chkpoint_two$$
$spell
    CppAD
    chk
    chkpoint
    hpp
    cppad
    bool
    hes
$$

$section Checkpoint Functions: Second Generation$$

$head Under Construction$$

$head Syntax$$
$codei%chkpoint_two<%Base%> %chk_fun%( %fun%, %name%,
    %internal_bool%, %use_hes_sparsity%, %use_base2ad%, %use_in_parallel%
)%$$
$icode%chk_fun%(%ax%, %ay%)
%$$
$icode%chk_fun%.new_dynamic(%dynamic%)
%$$

$head Reduce Memory$$
You can reduce the size of the tape and memory required for AD
using a checkpoint representation of a function
$latex g : \B{R}^n \rightarrow \B{R}^m$$.

$head Faster Recording$$
It may also reduce the time to make a recording if the same $latex g(x)$$
is used many times (with different values) during the
recording of an $codei%ADFun<%Base%>%$$ object.

$head Repeating Forward$$
Normally, CppAD stores $cref forward$$ mode results,
until they freed using $cref capacity_order$$,
or the corresponding $cref ADFun$$ object is deleted.
This is not true for $code chkpoint_two$$ functions
because the same checkpoint function may be used repeatedly
with different arguments during a single forward mode operation.
Thus, forward mode results are computed for each use of $icode chk_fun$$
in a forward mode sweep.

$head Operation Sequence$$
The $cref/operation sequence/glossary/Operation/Sequence/$$
representing $latex g(x)$$ is fixed; i.e.,
it cannot depend on the value of $latex x$$.

$head atomic_three$$
The $code chkpoint_two$$ class is derived from $code atomic_three$$,
hence some of its error message will refer to atomic operations.
The $code chkpoint_two$$ class implements all the
$cref/virtual functions/atomic_three/Virtual Functions/$$
and hence its source code,
$codei%
    include/cppad/core/chkpoint_two/chkpoint_two.hpp
%$$
provides an example for $cref atomic_three$$ operations.
The difference is that $code chkpoint_two.hpp$$ uses AD
instead of user provided derivatives.

$head Base$$
The type $icode Base$$ specifies the base type for AD operations;
i.e., $icode chk_fun$$ can be used during the recording of
$codei%AD<%Base%>%$$ operations.


$childtable%include/cppad/core/chkpoint_two/ctor.hpp
    %include/cppad/core/chkpoint_two/chk_fun.omh
    %example/chkpoint_two/get_started.cpp
    %example/chkpoint_two/simple.cpp
%$$

$end
*/

template <class Base>
class chkpoint_two : public atomic_three<Base> {
// ---------------------------------------------------------------------------
private:
    /// are sparsity calculations using bools or sets of integers
    const bool internal_bool_;
    //
    /// can this checkpoint function calculate Hessian sparsity patterns
    const bool use_hes_sparsity_;
    //
    /// can this checkpoint function be used in base2ad recordings
    const bool use_base2ad_;
    //
    /// can this checkpoint function be used in parallel mode
    const bool use_in_parallel_;
    //
    /// Jacobian sparsity for g(x) with dependncy true.
    /// This is set by the constructor and constant after that.
    sparse_rc< vector<size_t> > jac_sparsity_;
    //
    /// Hessian sparsity for g(x). If use_hes_sparsity_ is true,
    /// This is set by the constructor and constant after that.
    sparse_rc< vector<size_t> > hes_sparsity_;
    //
    /// Function corresponding to this checkpoint object.
    /// If use_in_parallel_, this is constant after the constructor.
    ADFun<Base>    g_;
    //
    /// AD version of function corresponding to this checkpoint object
    /// If use_in_parallel_, this is constant after the constructor.
    ADFun< AD<Base>, Base>  ag_;
    // ------------------------------------------------------------------------
    // member_
    // ------------------------------------------------------------------------
    /// If use_in_parallel_ is true, must have a separate copy member data
    /// that is not constant.
    struct member_struct {
        //
        /// function corresponding to this checkpoint object
        ADFun<Base>                 g_;
        //
        /// AD version of this function object
        ADFun< AD<Base>, Base >     ag_;
        //
    };
    /// use pointers and allocate memory to avoid false sharing
    /// (initialized to null by constructor)
    member_struct* member_[CPPAD_MAX_NUM_THREADS];
    //
    // ------------------------------------------------------------------------
    /// allocate member_ for this thread
    void allocate_member(size_t thread)
    {   CPPAD_ASSERT_UNKNOWN( use_in_parallel_ );
        if( member_[thread] == CPPAD_NULL )
        {   // Other threds have copy of corresponding informaiton.
            member_[thread]->g_  = g_;
            member_[thread]->ag_ = ag_;
        }
        return;
    }
    //
    // ------------------------------------------------------------------------
    /// free member_ for this thread
    void free_member(size_t thread)
    {   if( member_[thread] != CPPAD_NULL )
        {   delete member_[thread];
            member_[thread] = CPPAD_NULL;
        }
        return;
    }
    // -----------------------------------------------------------------------
    // atomic_three virtual functions
    // ------------------------------------------------------------------------
    // type
    virtual bool type(
        const vector<Base>&          parameter_x ,
        const vector<ad_type_enum>&  type_x      ,
        vector<ad_type_enum>&        type_y
    );
    // forward
    virtual bool forward(
        size_t                       need_y     ,
        size_t                       order_low  ,
        size_t                       order_up   ,
        const vector<ad_type_enum>&  type_x     ,
        const vector<Base>&          taylor_x   ,
        vector<Base>&                taylor_y
    );
    // AD forward
    virtual bool forward(
        size_t                       need_y     ,
        size_t                       order_low  ,
        size_t                       order_up   ,
        const vector<ad_type_enum>&  type_x     ,
        const vector< AD<Base> >&    taylor_x   ,
        vector< AD<Base> >&          taylor_y
    );
    // reverse
    virtual bool reverse(
        size_t                    order_up   ,
        const vector<Base>&       taylor_x   ,
        const vector<Base>&       taylor_y   ,
        vector<Base>&             partial_x  ,
        const vector<Base>&       partial_y
    );
    // AD reverse
    virtual bool reverse(
        size_t                     order_up   ,
        const vector< AD<Base> >&  ataylor_x   ,
        const vector< AD<Base> >&  ataylor_y   ,
        vector< AD<Base> >&        apartial_x  ,
        const vector< AD<Base> >&  apartial_y
    );
    // jac_sparsity
    virtual bool jac_sparsity(
        bool                           dependency   ,
        const vector<Base>&            parameter_x  ,
        const vector<ad_type_enum>&    type_x       ,
        const vector<bool>&            select_x     ,
        const vector<bool>&            select_y     ,
        sparse_rc< vector<size_t> >&   pattern_out
    );
    // hes_sparsity
    virtual bool hes_sparsity(
        const vector<Base>&            parameter_x  ,
        const vector<ad_type_enum>&    type_x       ,
        const vector<bool>&            select_x     ,
        const vector<bool>&            select_y     ,
        sparse_rc< vector<size_t> >&   pattern_out
    );
public:
    // ctor
    chkpoint_two(
        const ADFun<Base>& fun    ,
        const std::string& name   ,
        bool  internal_bool       ,
        bool  use_hes_sparsity    ,
        bool  use_base2ad         ,
        bool  use_in_parallel
    );
    //
    // destructor
    ~chkpoint_two(void);
    //
    // new_dynamic
    template <class BaseVector>
    void new_dynamic(const BaseVector& dynamic);
};

} // END_CPPAD_NAMESPACE

# include <cppad/core/chkpoint_two/ctor.hpp>
# include <cppad/core/chkpoint_two/type.hpp>
# include <cppad/core/chkpoint_two/forward.hpp>
# include <cppad/core/chkpoint_two/reverse.hpp>
# include <cppad/core/chkpoint_two/jac_sparsity.hpp>
# include <cppad/core/chkpoint_two/hes_sparsity.hpp>

# endif
