# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_LIN_ODE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_LIN_ODE_HPP

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
$begin atomic_four_lin_ode.hpp$$
$spell
$$

$section Atomic Linear ODE Class: Example Implementation$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
template <class Base>
class atomic_lin_ode : public CppAD::atomic_four<Base> {
//
public:
    // BEGIN sparse_rc_typedef
    typedef CppAD::sparse_rc< CppAD::vector<size_t> > sparse_rc;
    // END sparse_rc_typedef
    //
    // ctor
    atomic_lin_ode(const std::string& name) :
    CppAD::atomic_four<Base>(name)
    {   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
            work_[thread] = nullptr;
    }
    // destructor
    ~atomic_lin_ode(void)
    {   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
        {   if( work_[thread] != nullptr  )
            {   // allocated in set member function
                delete work_[thread];
            }
        }
    }
    // set
    size_t set(const Base& r, sparse_rc& pattern, const bool& transpose);
    //
    // get
    void get(size_t call_id, Base& r, sparse_rc& pattern, bool& transpose);
private:
    //
    // information connected to one call of this atomic function
    // pattern points to pattern_vec for this thread
    struct call_struct {
        size_t thread; Base r; size_t pattern_index; bool transpose;
    };
    //
    // information connected to each thread
    // patterns are in a separate vector so do not need one for every call
    struct thread_struct {
        CppAD::vector<sparse_rc>   pattern_vec;
        CppAD::vector<call_struct> call_vec;
    };
    //
    // Use pointers, to avoid false sharing between threads.
    thread_struct* work_[CPPAD_MAX_NUM_THREADS];
    //
    // base_lin_ode
    static void base_lin_ode(
        const Base&                r          ,
        const sparse_rc&           pattern    ,
        const bool&                transpose  ,
        const CppAD::vector<Base>& x          ,
        CppAD::vector<Base>&       y
    );
    //
    // -----------------------------------------------------------------------
    // overrides
    // -----------------------------------------------------------------------
# if 1
    //
    // for_type
    bool for_type(
        size_t                                        call_id,
        const CppAD::vector<CppAD::ad_type_enum>&     type_x,
        CppAD::vector<CppAD::ad_type_enum>&           type_y
    ) override;
    //
    // Base forward
    bool forward(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_y,
        size_t                                           order_low,
        size_t                                           order_up,
        const CppAD::vector<Base>&                       taylor_x,
        CppAD::vector<Base>&                             taylor_y
    ) override;
    // Cases not yet implemented
    //
    // AD<Base> forward
    bool forward(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_y,
        size_t                                           order_low,
        size_t                                           order_up,
        const CppAD::vector< CppAD::AD<Base> >&          ataylor_x,
        CppAD::vector< CppAD::AD<Base> >&                ataylor_y
    ) override;
    //
    // Base reverse
    bool reverse(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_x,
        size_t                                           order_up,
        const CppAD::vector<Base>&                       taylor_x,
        const CppAD::vector<Base>&                       taylor_y,
        CppAD::vector<Base>&                             partial_x,
        const CppAD::vector<Base>&                       partial_y
    ) override;
    //
    // AD<Base> reverse
    bool reverse(
        size_t                                           call_id,
        const CppAD::vector<bool>&                       select_x,
        size_t                                           order_up,
        const CppAD::vector< CppAD::AD<Base> >&          ataylor_x,
        const CppAD::vector< CppAD::AD<Base> >&          ataylor_y,
        CppAD::vector< CppAD::AD<Base> >&                apartial_x,
        const CppAD::vector< CppAD::AD<Base> >&          apartial_y
    ) override;
    //
    // jac_sparsity
    bool jac_sparsity(
        size_t                                         call_id,
        bool                                           dependency,
        const CppAD::vector<bool>&                     ident_zero_x,
        const CppAD::vector<bool>&                     select_x,
        const CppAD::vector<bool>&                     select_y,
        CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
    ) override;
    //
    // hes_sparsity
    bool hes_sparsity(
        size_t                                         call_id,
        const CppAD::vector<bool>&                     ident_zero_x,
        const CppAD::vector<bool>&                     select_x,
        const CppAD::vector<bool>&                     select_y,
        CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
    ) override;
# else
    //
    // rev_depend
    bool rev_depend(
        size_t                                         call_id,
        CppAD::vector<bool>&                           depend_x,
        const CppAD::vector<bool>&                     depend_y
    ) override;
# endif
};
} // END_CPPAD_NAMESPACE

# include <cppad/example/atomic_four/lin_ode/set.hpp>
# include <cppad/example/atomic_four/lin_ode/get.hpp>
# include <cppad/example/atomic_four/lin_ode/base_lin_ode.hpp>
# include <cppad/example/atomic_four/lin_ode/for_type.hpp>
# include <cppad/example/atomic_four/lin_ode/forward.hpp>
# include <cppad/example/atomic_four/lin_ode/reverse.hpp>
# include <cppad/example/atomic_four/lin_ode/jac_sparsity.hpp>
# include <cppad/example/atomic_four/lin_ode/hes_sparsity.hpp>
// END C++
# endif
