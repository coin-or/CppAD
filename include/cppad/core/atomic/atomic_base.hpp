# ifndef CPPAD_CORE_ATOMIC_ATOMIC_BASE_HPP
# define CPPAD_CORE_ATOMIC_ATOMIC_BASE_HPP
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
$begin atomic_base$$
$spell
    ctor
    cppad
    hpp
    afun
    arg
    vx
    vy
    tx
    ty
    px
    py
    jac
    hes
    CppAD
    checkpointing
    algo
$$

$section User Defined Atomic AD Functions$$


$head Syntax$$

$codei%
%atomic_user% %afun%(%ctor_arg_list%)
%afun%(%ax%, %ay%)
%ok% = %afun%.forward(%p%, %q%, %vx%, %vy%, %tx%, %ty%)
%ok% = %afun%.reverse(%q%, %tx%, %ty%, %px%, %py%)
%ok% = %afun%.for_sparse_jac(%q%, %r%, %s%, %x%)
%ok% = %afun%.rev_sparse_jac(%q%, %r%, %s%, %x%)
%ok% = %afun%.for_sparse_hes(%vx%, %r%, %s%, %h%, %x%)
%ok% = %afun%.rev_sparse_hes(%vx%, %s%, %t%, %q%, %r%, %u%, %v%, %x%)
atomic_base<%Base%>::clear()%$$

$head See Also$$
$cref checkpoint$$

$head Purpose$$

$subhead Speed$$
In some cases, the user knows how to compute derivatives of a function
$latex \[
    y = f(x) \; {\rm where} \; f : \B{R}^n \rightarrow \B{R}^m
\] $$
more efficiently than by coding it using $codei%AD<%Base%>%$$
$cref/atomic_base/glossary/Operation/Atomic/$$ operations
and letting CppAD do the rest.
In this case $codei%atomic_base%<%Base%>%$$ can use
the user code for $latex f(x)$$, and its derivatives,
as $codei%AD<%Base%>%$$ atomic operations.

$subhead Reduce Memory$$
If the function $latex f(x)$$ is used often,
using an atomic version of $latex f(x)$$ remove the need for repeated
copies of the corresponding $codei%AD<%Base%>%$$ operations.

$head Virtual Functions$$
User defined derivatives are implemented by defining the
following virtual functions in the $icode base_atomic$$ class:
$cref/forward/atomic_forward/$$,
$cref/reverse/atomic_reverse/$$,
$cref/for_sparse_jac/atomic_for_sparse_jac/$$,
$cref/rev_sparse_jac/atomic_rev_sparse_jac/$$, and
$cref/rev_sparse_hes/atomic_rev_sparse_hes/$$.
These virtual functions have a default implementation
that returns $icode%ok% == false%$$.
The $code forward$$ function,
for the case $icode%q% == 0%$$, must be implemented.
Otherwise, only those functions
required by the your calculations need to be implemented.
For example,
$icode forward$$ for the case $icode%q% == 2%$$ can just return
$icode%ok% == false%$$ unless you require
forward mode calculation of second derivatives.

$head Examples$$
See $cref atomic_example$$.

$childtable%
    include/cppad/core/atomic/ctor.hpp%
    include/cppad/core/atomic/option.hpp%
    include/cppad/core/atomic/afun.hpp%
    include/cppad/core/atomic/forward.hpp%
    include/cppad/core/atomic/reverse.hpp%
    include/cppad/core/atomic/for_sparse_jac.hpp%
    include/cppad/core/atomic/rev_sparse_jac.hpp%
    include/cppad/core/atomic/for_sparse_hes.hpp%
    include/cppad/core/atomic/rev_sparse_hes.hpp%
    include/cppad/core/atomic/clear.hpp
%$$

$end
-------------------------------------------------------------------------------
$begin atomic_example$$

$section Example User Defined Atomic Functions$$

$head Getting Started$$
The file $cref atomic_get_started.cpp$$ contains an example and test
that shows the minimal amount of information required to create
a user defined atomic operation.

$head Scalar Function$$
The file $cref atomic_reciprocal.cpp$$ contains an example and test
where the user provides the code for computing derivatives.
This example is simple because the domain and range are scalars.

$head Vector Range$$
The file $cref atomic_tangent.cpp$$ contains another example
where the user provides the code for computing derivatives.
This example is more complex because the range has two components.

$head Hessian Sparsity Patterns$$
The file $cref atomic_rev_sparse_hes.cpp$$ contains an minimal example
where the user provides the code for computing Hessian sparsity patterns.

$head General Case$$
The file $cref atomic_mat_mul.cpp$$ contains a more general example
where the user provides the code for computing derivatives.
This example is more complex because both the domain and range
dimensions are arbitrary.

$childtable%
    example/atomic/get_started.cpp%
    example/atomic/norm_sq.cpp%
    example/atomic/reciprocal.cpp%
    example/atomic/set_sparsity.cpp%
    example/atomic/tangent.cpp%
    example/atomic/eigen_mat_mul.cpp%
    example/atomic/eigen_mat_inv.cpp%
    example/atomic/eigen_cholesky.cpp%
    example/atomic/mat_mul.cpp%
    example/atomic/base2ad.cpp
%$$

$end
-------------------------------------------------------------------------------
*/

# include <set>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/sparse_internal.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic_base.hpp
Base class for atomic function operations.
*/

template <class Base>
class atomic_base {
// ===================================================================
public:
    enum option_enum {
        pack_sparsity_enum   ,
        bool_sparsity_enum   ,
        set_sparsity_enum
    };
private:
    // ------------------------------------------------------
    // constants
    //
    /// index of this object in class_object
    const size_t index_;

    // -----------------------------------------------------
    // variables
    //
    /// sparsity pattern this object is currently using
    /// (set by constructor and option member functions)
    option_enum sparsity_;

    /// temporary work space used by member functions, declared here to avoid
    // memory allocation/deallocation for each usage
    struct work_struct {
        vector<bool>               vx;
        vector<bool>               vy;
        //
        vector<Base>               tx;
        vector<Base>               ty;
        //
        vector< AD<Base> >         atx;
        vector< AD<Base> >         aty;
        //
        vector<bool>               bool_t;
        //
        vectorBool                 pack_h;
        vectorBool                 pack_r;
        vectorBool                 pack_s;
        vectorBool                 pack_u;
        //
        vector<bool>               bool_h;
        vector<bool>               bool_r;
        vector<bool>               bool_s;
        vector<bool>               bool_u;
        //
        vector< std::set<size_t> > set_h;
        vector< std::set<size_t> > set_r;
        vector< std::set<size_t> > set_s;
        vector< std::set<size_t> > set_u;
    };
    // Use pointers, to avoid false sharing between threads.
    // Not using: vector<work_struct*> work_;
    // so that deprecated atomic examples do not result in a memory leak.
    work_struct* work_[CPPAD_MAX_NUM_THREADS];
    // -----------------------------------------------------
    // static member functions
    //
    /// List of all the object in this class
    static std::vector<atomic_base *>& class_object(void)
    {   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
        static std::vector<atomic_base *> list_;
        return list_;
    }
    /// List of names for each object in this class
    static std::vector<std::string>& class_name(void)
    {   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
        static std::vector<std::string> list_;
        return list_;
    }
public:
    // =====================================================================
    // In User API
    // =====================================================================
    //
    // ---------------------------------------------------------------------
    // ctor: doxygen in atomic_base/ctor.hpp
    atomic_base(void);
    atomic_base(
        const std::string&     name,
        option_enum            sparsity = bool_sparsity_enum
    );

    // option: see doxygen in atomic_base/option.hpp
    void option(enum option_enum option_value);

    // operator(): see doxygen in atomic_base/afun.hpp
    template <class ADVector>
    void operator()(
        const ADVector&  ax     ,
              ADVector&  ay     ,
        size_t           id = 0
    );

    // ------------------------------------------------------------------------
    // forward: see docygen in atomic_base/forward.hpp
    virtual bool forward(
        size_t                    p  ,
        size_t                    q  ,
        const vector<bool>&       vx ,
              vector<bool>&       vy ,
        const vector<Base>&       tx ,
              vector<Base>&       ty
    );
    virtual bool forward(
        size_t                    p  ,
        size_t                    q  ,
        const vector<bool>&       vx ,
              vector<bool>&       vy ,
        const vector< AD<Base> >& atx ,
              vector< AD<Base> >& aty
    );

    // ------------------------------------------------------------------------
    // reverse: see docygen in atomic_base/reverse.hpp
    virtual bool reverse(
        size_t                    q  ,
        const vector<Base>&       tx ,
        const vector<Base>&       ty ,
              vector<Base>&       px ,
        const vector<Base>&       py
    );
    virtual bool reverse(
        size_t                    q   ,
        const vector< AD<Base> >& atx ,
        const vector< AD<Base> >& aty ,
              vector< AD<Base> >& apx ,
        const vector< AD<Base> >& apy
    );

    // ------------------------------------------------------------
    // for_sparse_jac: see doxygen in atomic_base/for_sparse_jac.hpp
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
              vector< std::set<size_t> >&       s  ,
        const vector<Base>&                     x
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     r  ,
              vector<bool>&                     s  ,
        const vector<Base>&                     x
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       r  ,
              vectorBool&                       s  ,
        const vector<Base>&                     x
    );
    template <class InternalSparsity>
    void for_sparse_jac(
        const vector<Base>&              x            ,
        const local::pod_vector<size_t>& x_index      ,
        const local::pod_vector<size_t>& y_index      ,
        InternalSparsity&                var_sparsity
    );
    // deprecated versions
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
              vector< std::set<size_t> >&       s
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     r  ,
              vector<bool>&                     s
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       r  ,
              vectorBool&                       s
    );
    // ------------------------------------------------------------
    // rev_sparse_jac: see doxygen in atomic_base/rev_sparse_jac.hpp
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       rt ,
              vector< std::set<size_t> >&       st ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     rt ,
              vector<bool>&                     st ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       rt ,
              vectorBool&                       st ,
        const vector<Base>&                     x
    );
    template <class InternalSparsity>
    void rev_sparse_jac(
        const vector<Base>&        x            ,
        const local::pod_vector<size_t>& x_index ,
        const local::pod_vector<size_t>& y_index ,
        InternalSparsity&          var_sparsity
    );
    // deprecated versions
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       rt ,
              vector< std::set<size_t> >&       st
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     rt ,
              vector<bool>&                     st
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       rt ,
              vectorBool&                       st
    );
    // ------------------------------------------------------------
    // for_sparse_hes: see doxygen in atomic_base/for_sparse_hes.hpp
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector< std::set<size_t> >&     h  ,
        const vector<Base>&             x
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector<bool>&                   h  ,
        const vector<Base>&             x
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vectorBool&                     h  ,
        const vector<Base>&             x
    );
    template <class InternalSparsity>
    void for_sparse_hes(
        const vector<Base>&              x                ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        const InternalSparsity&          for_jac_sparsity ,
        const InternalSparsity&          rev_jac_sparsity ,
        InternalSparsity&                for_hes_sparsity
    );
    // deprecated versions
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector< std::set<size_t> >&     h
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector<bool>&                   h
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vectorBool&                     h
    );
    // ------------------------------------------------------------
    // rev_sparse_hes: see doxygen in atomic_base/rev_sparse_hes.hpp
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
        const vector< std::set<size_t> >&       u  ,
              vector< std::set<size_t> >&       v  ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector<bool>&                     r  ,
        const vector<bool>&                     u  ,
              vector<bool>&                     v  ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vectorBool&                       r  ,
        const vectorBool&                       u  ,
              vectorBool&                       v  ,
        const vector<Base>&                     x
    );
    template <class InternalSparsity>
    void rev_sparse_hes(
        const vector<Base>&              x                ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        const InternalSparsity&          for_jac_sparsity ,
        bool*                            rev_jac_flag     ,
        InternalSparsity&                rev_hes_sparsity
    );
    // deprecated
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
        const vector< std::set<size_t> >&       u  ,
              vector< std::set<size_t> >&       v
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector<bool>&                     r  ,
        const vector<bool>&                     u  ,
              vector<bool>&                     v
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vectorBool&                       r  ,
        const vectorBool&                       u  ,
              vectorBool&                       v
    );
    // ------------------------------------------------------------
    // clear: see doxygen in atomic_base/clear.hpp
    static void clear(void);

    // =====================================================================
    // Not in User API
    // =====================================================================

    /// current sparsity setting
    option_enum sparsity(void) const
    {   return sparsity_; }

    /// Name corresponding to a base_atomic object
    const std::string& afun_name(void) const
    {   return class_name()[index_]; }

    /// destructor informs CppAD that this atomic function with this index
    /// has dropped out of scope by setting its pointer to null
    virtual ~atomic_base(void)
    {   CPPAD_ASSERT_UNKNOWN( class_object().size() > index_ );
        // change object pointer to null, but leave name for error reporting
        class_object()[index_] = CPPAD_NULL;
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
    /// atomic_base function object corresponding to a certain index
    static atomic_base* class_object(size_t index)
    {   CPPAD_ASSERT_UNKNOWN( class_object().size() > index );
        return class_object()[index];
    }
    /// atomic_base function name corresponding to a certain index
    static const std::string& class_name(size_t index)
    {   CPPAD_ASSERT_UNKNOWN( class_name().size() > index );
        return class_name()[index];
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

// functitons implemented in cppad/core/atomic_base files
# include <cppad/core/atomic/ctor.hpp>
# include <cppad/core/atomic/option.hpp>
# include <cppad/core/atomic/afun.hpp>
# include <cppad/core/atomic/forward.hpp>
# include <cppad/core/atomic/reverse.hpp>
# include <cppad/core/atomic/for_sparse_jac.hpp>
# include <cppad/core/atomic/rev_sparse_jac.hpp>
# include <cppad/core/atomic/for_sparse_hes.hpp>
# include <cppad/core/atomic/rev_sparse_hes.hpp>
# include <cppad/core/atomic/clear.hpp>

# endif
