# ifndef CPPAD_CORE_ATOMIC_FOUR_ATOMIC_HPP
# define CPPAD_CORE_ATOMIC_FOUR_ATOMIC_HPP
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
$begin atomic_four_define$$
$spell
    taylor
    ctor
    afun
    arg
    jac
    hes
    CppAD
    enum
    mul
    hpp
    const
    ident
$$

$section Defining Atomic Functions: Fourth Generation$$

$head Syntax$$

$subhead Define Class$$
$codei%class %atomic_user% : public CppAD::atomic_four<%Base%> {
    %...%
};%$$

$subhead Constructor$$
$icode%atomic_user% %afun%(%ctor_arg_list%)%$$

$subhead Call$$
$icode%afun%(%ax%, %ay%)
%$$
$icode%afun%(%call_id%, %ax%, %ay%)
%$$

$subhead Callbacks$$
$icode%ok% = %afun%.for_type( %call_id%,
    %type_x%, %type_y%
)
%ok% = %afun%.forward( %call_id%,
    %select_y%, %order_low%, %order_up%, %taylor_x%, %taylor_y%
)
%ok% = %afun%.reverse( %call_id%,
    %select_x%, %order_up%, %taylor_x%, %taylor_y%, %partial_x%, %partial_y%
)
%ok% = %afun%.jac_sparsity( %call_id%,
    %dependency%, %ident_zero_x%, %select_x% %select_y%, %pattern_out%
)
%ok% = %afun%.hes_sparsity( %call_id%,
    %ident_zero_x%, %select_x% %select_y%, %pattern_out%
)
%ok% = %afun%.rev_depend( %call_id%,
    %depend_x%, %depend_y%
)%$$

$head See Also$$
$cref chkpoint_two$$, $cref atomic_three$$

$head Purpose$$

$subhead Speed$$
In some cases, it is possible to compute derivatives of a function
$latex \[
    y = g(x) \; {\rm where} \; g : \B{R}^n \rightarrow \B{R}^m
\] $$
more efficiently than by coding it using $codei%AD<%Base%>%$$
$cref/atomic/glossary/Operation/Atomic/$$ operations
and letting CppAD do the rest.
The class $codei%atomic_four%<%Base%>%$$ is used to
create a new atomic operation corresponding to a function $latex g(x)$$
where the user specifies how to compute the derivatives
and sparsity patterns for $latex g(x)$$.

$subhead Reduce Memory$$
If the function $latex g(x)$$ is used many times during the recording
of an $cref ADFun$$ object,
an atomic version of $latex g(x)$$ removes the need for repeated
copies of the corresponding $codei%AD<%Base%>%$$ operations and variables
in the recording.

$head Virtual Functions$$
The $cref/callback functions/atomic_four_define/Syntax/Callbacks/$$
are implemented by defining the virtual functions in the
$icode atomic_user$$ class.
These functions compute derivatives,
sparsity patterns, and dependency relations.
Each virtual function has a default implementation
that returns $icode%ok% == false%$$.
The $cref/for_type/atomic_four_for_type/$$
and $cref/forward/atomic_four_forward/$$ function
(for the case $icode%order_up% == 0%$$) are used by an atomic function
$cref/call/atomic_four_define/Syntax/Call/$$.
Hence, they are required for one to use an atomic function.
Other functions and orders are only required if they are used
for your calculations.
For example,
$icode forward$$ for the case $icode%order_up% == 2%$$ can just return
$icode%ok% == false%$$ unless you require
forward mode calculation of second derivatives.

$childtable%include/cppad/core/atomic/four/ctor.hpp
    %include/cppad/core/atomic/four/call.hpp
    %include/cppad/core/atomic/four/for_type.hpp
    %include/cppad/core/atomic/four/forward.hpp
    %include/cppad/core/atomic/four/reverse.hpp
    %include/cppad/core/atomic/four/jac_sparsity.hpp
    %include/cppad/core/atomic/four/hes_sparsity.hpp
    %include/cppad/core/atomic/four/rev_depend.hpp
%$$

$end
-------------------------------------------------------------------------------
*/

# include <set>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/atomic_index.hpp>

// needed before one can use in_parallel
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic_four.hpp
Base class for atomic function operations.
*/

template <class Base>
class atomic_four {
// ===================================================================
private:
    // ------------------------------------------------------
    // constants
    //
    /// index of this object in lcal::atomic_index
    /// (set by constructor and not changed; i.e., effectively const)
    size_t index_;
    //
    // -----------------------------------------------------
    //
    /// temporary work space used by call member functions, declared here
    // to avoid memory allocation/deallocation for each call
    struct work_struct {
        vector<ad_type_enum>        type_x;
        vector<ad_type_enum>        type_y;
        //
        vector<Base>                taylor_x;
        vector<Base>                taylor_y;
        //
        vector< AD<Base> >          ataylor_x;
        vector< AD<Base> >          ataylor_y;
        //
        vector<bool>                select_y;
    };
    // Use pointers, to avoid false sharing between threads.
    // Not using: vector<work_struct*> work_;
    // so that deprecated atomic examples do not result in a memory leak.
    work_struct* work_[CPPAD_MAX_NUM_THREADS];
    // -----------------------------------------------------
public:
    // =====================================================================
    // In User API
    // =====================================================================
    //
    // ---------------------------------------------------------------------
    // constructors
    atomic_four(void);
    atomic_four(const std::string& name);

    // ------------------------------------------------------------------------
    template <class ADVector> void operator()(
        size_t           call_id ,
        const ADVector&  ax      ,
              ADVector&  ay
    );
    template <class ADVector> void operator()(
        const ADVector&  ax      ,
              ADVector&  ay
    );
    // ------------------------------------------------------------------------
    // for_type
    virtual bool for_type(
        size_t                       call_id     ,
        const vector<ad_type_enum>&  type_x      ,
        vector<ad_type_enum>&        type_y
    );
    // ------------------------------------------------------------------------
    // rev_depend
    virtual bool rev_depend(
        size_t                       call_id     ,
        vector<bool>&                depend_x    ,
        const vector<bool>&          depend_y
    );
    // ------------------------------------------------------------------------
    // forward
    virtual bool forward(
        size_t                       call_id     ,
        const vector<bool>&          select_y    ,
        size_t                       order_low   ,
        size_t                       order_up    ,
        const vector<Base>&          taylor_x    ,
        vector<Base>&                taylor_y
    );
    virtual bool forward(
        size_t                       call_id      ,
        const vector<bool>&          select_y    ,
        size_t                       order_low    ,
        size_t                       order_up     ,
        const vector< AD<Base> >&    ataylor_x    ,
        vector< AD<Base> >&          ataylor_y
    );
    // ------------------------------------------------------------------------
    // reverse
    virtual bool reverse(
        size_t                       call_id     ,
        const vector<bool>&          select_x    ,
        size_t                       order_up    ,
        const vector<Base>&          taylor_x    ,
        const vector<Base>&          taylor_y    ,
        vector<Base>&                partial_x   ,
        const vector<Base>&          partial_y
    );
    virtual bool reverse(
        size_t                       call_id     ,
        const vector<bool>&          select_x    ,
        size_t                       order_up    ,
        const vector< AD<Base> >&    ataylor_x   ,
        const vector< AD<Base> >&    ataylor_y   ,
        vector< AD<Base> >&          apartial_x  ,
        const vector< AD<Base> >&    apartial_y
    );
    // ------------------------------------------------------------
    // jac_sparsity
    virtual bool jac_sparsity(
        size_t                       call_id      ,
        bool                         dependency   ,
        const vector<bool>&          ident_zero_x ,
        const vector<bool>&          select_x     ,
        const vector<bool>&          select_y     ,
        sparse_rc< vector<size_t> >& pattern_out
    );
    template <class InternalSparsity>
    bool for_jac_sparsity(
        size_t                           call_id      ,
        bool                             dependency   ,
        const vector<bool>&              ident_zero_x ,
        const local::pod_vector<size_t>& x_index      ,
        const local::pod_vector<size_t>& y_index      ,
        InternalSparsity&                var_sparsity
    );
    template <class InternalSparsity>
    bool rev_jac_sparsity(
        size_t                           call_id      ,
        bool                             dependency   ,
        const vector<bool>&              ident_zero_x ,
        const local::pod_vector<size_t>& x_index      ,
        const local::pod_vector<size_t>& y_index      ,
        InternalSparsity&                var_sparsity
    );
    // deprecated version of this callback
    virtual bool jac_sparsity(
        size_t                       call_id      ,
        bool                         dependency   ,
        const vector<bool>&          select_x     ,
        const vector<bool>&          select_y     ,
        sparse_rc< vector<size_t> >& pattern_out
    );
    // ------------------------------------------------------------
    // hes_sparsity
    virtual bool hes_sparsity(
        size_t                                  call_id      ,
        const vector<bool>&                     ident_zero_x ,
        const vector<bool>&                     select_x     ,
        const vector<bool>&                     select_y     ,
        sparse_rc< vector<size_t> >&            pattern_out
    );
    template <class InternalSparsity>
    bool for_hes_sparsity(
        size_t                           call_id          ,
        const vector<bool>&              ident_zero_x     ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        size_t                           np1              ,
        size_t                           numvar           ,
        const InternalSparsity&          rev_jac_sparsity ,
        InternalSparsity&                for_sparsity
    );
    template <class InternalSparsity>
    bool rev_hes_sparsity(
        size_t                           call_id          ,
        const vector<bool>&              ident_zero_x     ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        const InternalSparsity&          for_jac_pattern  ,
        bool*                            rev_jac_flag     ,
        InternalSparsity&                hes_sparsity
    );
    // deprecated version of this callback
    virtual bool hes_sparsity(
        size_t                                  call_id      ,
        const vector<bool>&                     select_x     ,
        const vector<bool>&                     select_y     ,
        sparse_rc< vector<size_t> >&            pattern_out
    );
    // =====================================================================
    // Not in User API
    // =====================================================================

    /// Name corresponding to a atomic_four object
    const std::string atomic_name(void) const
    {   bool        set_null = false;
        size_t      type  = 0;          // set to avoid warning
        std::string name;
        void*       v_ptr = nullptr; // set to avoid warning
        local::atomic_index<Base>(set_null, index_, type, &name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 4 );
        return name;
    }
    /// destructor informs CppAD that this atomic function with this index
    /// has dropped out of scope by setting its pointer to null
    virtual ~atomic_four(void)
    {   // change object pointer to null, but leave name for error reporting
        bool         set_null = true;
        size_t       type  = 0;          // set to avoid warning
        std::string* name  = nullptr;
        void*        v_ptr = nullptr; // set to avoid warning
        local::atomic_index<Base>(set_null, index_, type, name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 4 );
        //
        // free temporary work memory
        for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
            free_work(thread);
    }
    /// allocates work_ for a specified thread
    void allocate_work(size_t thread)
    {   if( work_[thread] == nullptr )
        {   // allocate the raw memory
            size_t min_bytes = sizeof(work_struct);
            size_t num_bytes;
            void*  v_ptr     = thread_alloc::get_memory(min_bytes, num_bytes);
            // save in work_
            work_[thread]    = reinterpret_cast<work_struct*>( v_ptr );
            // call constructor
            new( work_[thread] ) work_struct;
        }
        return;
    }
    /// frees work_ for a specified thread
    void free_work(size_t thread)
    {   if( work_[thread] != nullptr )
        {   // call destructor
            work_[thread]->~work_struct();
            // return memory to avialable pool for this thread
            thread_alloc::return_memory(
                reinterpret_cast<void*>(work_[thread])
            );
            // mark this thread as not allocated
            work_[thread] = nullptr;
        }
        return;
    }
    /// atomic_four function object corresponding to a certain index
    static atomic_four* class_object(size_t index)
    {   bool         set_null = false;
        size_t       type  = 0;          // set to avoid warning
        std::string* name  = nullptr;
        void*        v_ptr = nullptr; // set to avoid warning
        local::atomic_index<Base>(set_null, index, type, name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 4 );
        return reinterpret_cast<atomic_four*>( v_ptr );
    }
    /// atomic_four function name corresponding to a certain index
    static const std::string class_name(size_t index)
    {   bool        set_null = false;
        size_t      type  = 0;          // set to avoid warning
        std::string name;
        void*       v_ptr = nullptr; // set to avoid warning
        local::atomic_index<Base>(set_null, index, type, &name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 4 );
        return name;
    }

    /*!
    Set value of id (used by deprecated atomic_one class)

    This function is called just before calling any of the virtual function
    and has the corresponding id of the corresponding virtual call.
    */
    virtual void set_old(size_t id)
    { }
// ---------------------------------------------------------------------------
};
} // END_CPPAD_NAMESPACE

// member functions
# include <cppad/core/atomic/four/ctor.hpp>
# include <cppad/core/atomic/four/call.hpp>
# include <cppad/core/atomic/four/for_type.hpp>
# include <cppad/core/atomic/four/rev_depend.hpp>
# include <cppad/core/atomic/four/forward.hpp>
# include <cppad/core/atomic/four/reverse.hpp>
# include <cppad/core/atomic/four/jac_sparsity.hpp>
# include <cppad/core/atomic/four/hes_sparsity.hpp>

# endif
