# ifndef CPPAD_CORE_ATOMIC_ATOMIC_THREE_HPP
# define CPPAD_CORE_ATOMIC_ATOMIC_THREE_HPP
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
$begin atomic_three$$
$spell
    taylor
    ctor
    afun
    arg
    jac
    hes
    CppAD
    enum
$$

$section Defining Atomic Functions: Third Generation$$

$head Syntax$$

$codei%
%atomic_derived% %afun%(%ctor_arg_list%)
%afun%(%ax%, %ay%)
%ok% = %afun%.type(%parameter_x%, %type_x%, %type_y%)
%ok% = %afun%.forward(
    %need_y%, %order_low%, %order_up%, %type_x%, %taylor_x%, %taylor_y%
)
%ok% = %afun%.reverse(
    %order_up%, %taylor_x%, %taylor_y%, %partial_x%, %partial_y%
)
%ok% = %afun%.jac_sparsity(
    %dependency%, %parameter_x%, %type_x%, %select_x% %select_y%, %pattern_out%
)
%ok% = %afun%.hes_sparsity(
    %parameter_x%, %select_x% %select_y%, %pattern_out%
)
atomic_three<%Base%>::clear()%$$

$head See Also$$
$cref checkpoint$$, $cref atomic_two$$

$head Purpose$$

$subhead Speed$$
In some cases, it is possible to compute derivatives of a function
$latex \[
    y = g(x) \; {\rm where} \; g : \B{R}^n \rightarrow \B{R}^m
\] $$
more efficiently than by coding it using $codei%AD<%Base%>%$$
$cref/atomic/glossary/Operation/Atomic/$$ operations
and letting CppAD do the rest.
The class $codei%atomic_three%<%Base%>%$$ is used to
create a new atomic operation corresponding to a function $latex g(x)$$
where the user specifies how to compute the derivatives
and sparsity patterns for $latex g(x)$$.

$subhead Reduce Memory$$
If the function $latex g(x)$$ is many times during the recording
of an $cref ADFun$$ object,
using an atomic version of $latex g(x)$$ removed the need for repeated
copies of the corresponding $codei%AD<%Base%>%$$ operations and variables
in the recording.

$head Virtual Functions$$
Derivatives for an $code atomic_three$$
function are implemented by defining the
following virtual functions in the $icode atomic_derived$$ class:
$comment 2DO:
1. Change name of atomic_base versions of these sections.
2. Document and implement these new sections.
3. Copy atomic_base examples to test_more/deprecated.
4. Change atomic_base examples to use base_three.
$$
$cref/forward/atomic_two_forward/$$,
$cref/reverse/atomic_two_reverse/$$,
$code jac_sparsity$$,
$code hes_sparsity$$,
These virtual functions have a default implementation
that returns $icode%ok% == false%$$.
The $code forward$$ function,
for the case $icode%order_up% == 0%$$, must be implemented.
Otherwise, only those functions and orders
required by the your calculations need to be implemented.
For example,
$icode forward$$ for the case $icode%order_up% == 2%$$ can just return
$icode%ok% == false%$$ unless you require
forward mode calculation of second derivatives.

$head ad_type$$
The type $code CppAD::ad_type_enum$$
is used to specify if an AD object is a
$cref/constant parameter/glossary/Parameter/Constant/$$
$cref/dynamic parameter/glossary/Parameter/Dynamic/$$
or $cref/variable/glossary/Variable/$$.
It has the following possible values:
$center
$table
$icode ad_type_enum$$  $pre  $$   $cnext Meaning $rnext
$code constant_enum$$  $pre  $$   $cnext constant parameter $rnext
$code dynamic_enum$$   $pre  $$   $cnext dynamic parameter  $rnext
$code variable_enum$$  $pre  $$   $cnext variable
$tend
$$
In addition,
$code constant_enum < dynamic_enum < variable_enum$$.

$childtable%include/cppad/core/atomic/three_ctor.hpp
    %include/cppad/core/atomic/three_afun.hpp
    %include/cppad/core/atomic/three_type.hpp
    %include/cppad/core/atomic/three_forward.hpp
    %include/cppad/core/atomic/three_reverse.hpp
    %include/cppad/core/atomic/three_jac_sparsity.hpp
    %include/cppad/core/atomic/three_hes_sparsity.hpp
%$$

$end
-------------------------------------------------------------------------------
$begin atomic_three_example$$

$section Example Defining Atomic Functions: Third Generation$$

$childtable%example/atomic_three/get_started.cpp
    %example/atomic_three/norm_sq.cpp
    %example/atomic_three/tangent.cpp
    %example/atomic_three/base2ad.cpp
    %example/atomic_three/reciprocal.cpp
    %example/atomic_three/mat_mul.cpp
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
\file atomic_three.hpp
Base class for atomic function operations.
*/

template <class Base>
class atomic_three {
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
    /// temporary work space used by member functions, declared here to avoid
    // memory allocation/deallocation for each usage
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
        sparse_rc< vector<size_t> > pattern;
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
    // ctor: doxygen in atomic/three_ctor.hpp
    atomic_three(void);
    atomic_three(const std::string& name);

    // ------------------------------------------------------------------------
    // operator(): see doxygen in atomic_three/afun.hpp
    template <class ADVector>
    void operator()(
        const ADVector&  ax     ,
              ADVector&  ay
    );
    // ------------------------------------------------------------------------
    // type: doxygen in atomic/three_type.hpp
    virtual bool type(
        const vector<Base>&          parameter_x ,
        const vector<ad_type_enum>&  type_x      ,
        vector<ad_type_enum>&        type_y
    );
    // ------------------------------------------------------------------------
    // forward: see docygen in atomic/three_forward.hpp
    virtual bool forward(
        size_t                       need_y     ,
        size_t                       order_low  ,
        size_t                       order_up   ,
        const vector<ad_type_enum>&  type_x     ,
        const vector<Base>&          taylor_x   ,
        vector<Base>&                taylor_y
    );
    virtual bool forward(
        size_t                       need_y     ,
        size_t                       order_low  ,
        size_t                       order_up   ,
        const vector<ad_type_enum>&  type_x     ,
        const vector< AD<Base> >&    ataylor_x  ,
        vector< AD<Base> >&          ataylor_y
    );
    // ------------------------------------------------------------------------
    // reverse: see docygen in atomic/three_reverse.hpp
    virtual bool reverse(
        size_t                     order_up   ,
        const vector<Base>&        taylor_x   ,
        const vector<Base>&        taylor_y   ,
        vector<Base>&              partial_x  ,
        const vector<Base>&        partial_y
    );
    virtual bool reverse(
        size_t                     order_up    ,
        const vector< AD<Base> >&  ataylor_x   ,
        const vector< AD<Base> >&  ataylor_y   ,
        vector< AD<Base> >&        apartial_x  ,
        const vector< AD<Base> >&  apartial_y
    );
    // ------------------------------------------------------------
    // jac_sparsity: see doxygen in atomic/three_jac_sparsity.hpp
    virtual bool jac_sparsity(
        bool                         dependency  ,
        const vector<Base>&          parameter_x ,
        const vector<ad_type_enum>&  type_x      ,
        const vector<bool>&          select_x    ,
        const vector<bool>&          select_y    ,
        sparse_rc< vector<size_t> >& pattern_out
    );
    template <class InternalSparsity>
    bool for_jac_sparsity(
        bool                             dependency   ,
        const vector<Base>&              parameter_x  ,
        const vector<ad_type_enum>&      type_x       ,
        const local::pod_vector<size_t>& x_index      ,
        const local::pod_vector<size_t>& y_index      ,
        InternalSparsity&                var_sparsity
    );
    template <class InternalSparsity>
    bool rev_jac_sparsity(
        bool                             dependency   ,
        const vector<Base>&              parameter_x  ,
        const vector<ad_type_enum>&      type_x       ,
        const local::pod_vector<size_t>& x_index      ,
        const local::pod_vector<size_t>& y_index      ,
        InternalSparsity&                var_sparsity
    );
    // ------------------------------------------------------------
    // hes_sparsity: see doxygen in atomic/three_jac_sparsity.hpp
    virtual bool hes_sparsity(
        const vector<Base>&                     parameter_x  ,
        const vector<ad_type_enum>&             type_x       ,
        const vector<bool>&                     select_x     ,
        const vector<bool>&                     select_y     ,
        sparse_rc< vector<size_t> >&            pattern_out
    );
    template <class InternalSparsity>
    bool for_hes_sparsity(
        const vector<Base>&              parameter_x      ,
        const vector<ad_type_enum>&      type_x       ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        const InternalSparsity&          for_jac_sparsity ,
        const InternalSparsity&          rev_jac_sparsity ,
        InternalSparsity&                hes_sparsity
    );
    template <class InternalSparsity>
    bool rev_hes_sparsity(
        const vector<Base>&              parameter_x      ,
        const vector<ad_type_enum>&      type_x           ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        const InternalSparsity&          for_jac_sparsity ,
        bool*                            rev_jac_flag     ,
        InternalSparsity&                hes_sparsity
    );
/*
    // ------------------------------------------------------------
    // clear: see doxygen in atomic_three/clear.hpp
    static void clear(void);
*/

    // =====================================================================
    // Not in User API
    // =====================================================================

    /// Name corresponding to a atomic_three object
    const std::string afun_name(void) const
    {   bool        set_null = false;
        size_t      type  = 0;          // set to avoid warning
        std::string name;
        void*       v_ptr = CPPAD_NULL; // set to avoid warning
        local::atomic_index<Base>(set_null, index_, type, &name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 3 );
        return name;
    }
    /// destructor informs CppAD that this atomic function with this index
    /// has dropped out of scope by setting its pointer to null
    virtual ~atomic_three(void)
    {   // change object pointer to null, but leave name for error reporting
        bool         set_null = true;
        size_t       type  = 0;          // set to avoid warning
        std::string* name  = CPPAD_NULL;
        void*        v_ptr = CPPAD_NULL; // set to avoid warning
        local::atomic_index<Base>(set_null, index_, type, name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 3 );
        //
        // free temporary work memory
        for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
            free_work(thread);
    }
    /// allocates work_ for a specified thread
    void allocate_work(size_t thread)
    {   if( work_[thread] == CPPAD_NULL )
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
    {   if( work_[thread] != CPPAD_NULL )
        {   // call destructor
            work_[thread]->~work_struct();
            // return memory to avialable pool for this thread
            thread_alloc::return_memory(
                reinterpret_cast<void*>(work_[thread])
            );
            // mark this thread as not allocated
            work_[thread] = CPPAD_NULL;
        }
        return;
    }
    /// atomic_three function object corresponding to a certain index
    static atomic_three* class_object(size_t index)
    {   bool         set_null = false;
        size_t       type  = 0;          // set to avoid warning
        std::string* name  = CPPAD_NULL;
        void*        v_ptr = CPPAD_NULL; // set to avoid warning
        local::atomic_index<Base>(set_null, index, type, name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 3 );
        return reinterpret_cast<atomic_three*>( v_ptr );
    }
    /// atomic_three function name corresponding to a certain index
    static const std::string class_name(size_t index)
    {   bool        set_null = false;
        size_t      type  = 0;          // set to avoid warning
        std::string name;
        void*       v_ptr = CPPAD_NULL; // set to avoid warning
        local::atomic_index<Base>(set_null, index, type, &name, v_ptr);
        CPPAD_ASSERT_UNKNOWN( type == 3 );
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
# include <cppad/core/atomic/three_ctor.hpp>
# include <cppad/core/atomic/three_afun.hpp>
# include <cppad/core/atomic/three_type.hpp>
# include <cppad/core/atomic/three_forward.hpp>
# include <cppad/core/atomic/three_reverse.hpp>
# include <cppad/core/atomic/three_jac_sparsity.hpp>
# include <cppad/core/atomic/three_hes_sparsity.hpp>

# endif
