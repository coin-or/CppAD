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
$$

$section Checkpoint Functions: Second Generation$$

$head Under Construction$$

$head Syntax$$

$subhead Construct chk_fun$$
$codei%chkpoint_two<%Base%> %chk_fun%(%name%, %ad_fun%)
%$$

$subhead Use chk_fun$$
$icode%chk_fun%(%ax%, %ay%)
%$$

$subhead New Dynamic$$
$icode%chk_fun%.new_dynamic(%dynamic%)
%$$

$head Prototype$$

$subhead Construct chk_fun$$
$srcfile%include/cppad/core/chkpoint_two/chkpoint_two.hpp%
    0%// BEGIN_CTOR_PROTOTYPE%// END_CTOR_PROTOTYPE%1
%$$

$subhead Use chk_fun$$
$srcfile%include/cppad/core/chkpoint_two/chkpoint_two.hpp%
    0%// BEGIN_USE_PROTOTYPE%// END_USE_PROTOTYPE%1
%$$

$subhead New Dynamic$$
$srcfile%include/cppad/core/chkpoint_two/chkpoint_two.hpp%
    0%// BEGIN_NEW_DYNAMIC_PROTOTYPE%// END_NEW_DYNAMIC_PROTOTYPE%1
%$$


$head See Also$$
$cref reverse_checkpoint.cpp$$, $cref atomic_three$$, $cref chkpoint_two$$

$head Purpose$$

$subhead Reduce Memory$$
You can reduce the size of the tape and memory required for AD
using a checkpoint representation of a function
$latex g : \B{R}^n \rightarrow \B{R}^m$$.

$subhead Faster Recording$$
It may also reduce the time to make a recording the same function
for different values of the independent variable.

$subhead Repeating Forward$$
Normally, CppAD stores $cref forward$$ mode results until they freed
using $cref capacity_order$$ or the corresponding $cref ADFun$$ object is
deleted. This is not true for $code chkpoint_two$$ functions
because a checkpoint the same function may be used repeatedly
with different arguments during a single forward mode operation.
Thus, forward mode results are computed for each use of $icode chk_fun$$
in a forward mode sweep.

$subhead Operation Sequence$$
The $cref/operation sequence/glossary/Operation/Sequence/$$
representing $latex f(x)$$ is fixed; i.e.,
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

$head Construct chk_fun$$
This object $icode chk_fun$$ must not be destructed for as long
as any $codei%ADFun<%Base%>%$$ object uses its atomic operation.
Its constructor and destructor must not be called in
$cref/parallel/ta_in_parallel/$$ mode.

$subhead Base$$
The type $icode Base$$ specifies the base type for AD operations.

$subhead name$$
is the name used for reporting errors using this checkpoint function.

$subhead ad_fun$$
This specifies the operation sequence for this checkpoint function;
i.e. $latex g(x)$$.
Note that $icode ad_fun$$ may or may not have been
$cref/optimized/optimize/$$ before calling the constructor.
This will determine if the internal representation for $icode g(x)$$
is optimized.

$head Use chk_fun$$

$subhead ADVector$$
The type $icode ADVector$$ must be a
$cref/simple vector class/SimpleVector/$$ with elements of type
$codei%AD<%Base%>%$$.

$subhead ax$$
This argument has
size equal to $icode%n% = %ad_fun%.Domain()%$$.
It specifies vector $latex x \in \B{R}^n$$
at which we are computing an $codei%AD<%Base%>%$$ version of
$latex y = g(x)$$.

$subhead ay$$
This argument has
size must be equal to $icode%m% = %ad_fun%.Range()%$$.
The input values of its elements do not matter.
Upon return, it is an $codei%AD<%Base%>%$$ version of
$latex y = g(x)$$.


$head new_dynamic$$
It is possible to modify the definition of $latex g(x)$$
using the dynamic parameters in $icode ad_fun$$.

$subhead dynamic$$
This argument specifies a new value for the independent dynamic parameters in
the definition of $latex g(x)%$$.
It size must be the same as the independent
$cref/dynamic/Independent/dynamic/$$ vector corresponding to $icode ad_fun$$.
This changes the copy of $icode ad_fun$$ used by
$code chkpoint_two$$ but has not effect on the original $icode ad_fun$$.

$end

$head Example$$
The file $cref chkpoint_two.cpp$$ contains an example and test
of these operations.

$childtable%example/chkpoint_two/get_started.cpp
    %example/chkpoint_two/mul_level.cpp
    %example/chkpoint_two/ode.cpp
    %example/chkpoint_two/extended_ode.cpp
%$$

*/

template <class Base>
class chkpoint_two : public atomic_three<Base> {
// ---------------------------------------------------------------------------
private:
    // ------------------------------------------------------------------------
    // member_
    // ------------------------------------------------------------------------
    // same checkpoint object can be used by multiple threads
    struct member_struct {
        //
        /// AD function corresponding to this checkpoint object
        ADFun<Base>                 g_;
        ADFun< AD<Base>, Base >     ag_;
        //
        /// Jacobian sparsity for g(x)^{(1)} does not change so cache it
        sparse_rc< vector<size_t> > jac_sparsity_;
    };
    /// This version of work is const except during constructor
    member_struct const_member_;

    /// use pointers and allocate memory to avoid false sharing
    member_struct* member_[CPPAD_MAX_NUM_THREADS];
    //
    // ------------------------------------------------------------------------
    /// allocate member_ for this thread
    void allocate_member(size_t thread)
    {   if( member_[thread] == CPPAD_NULL )
        {   member_[thread] = new member_struct;
            // The function is recorded in sequential mode and placed in
            // const_member_.f_, other threads have copy.
            member_[thread]->f_ = const_member_.f_;
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
    // ------------------------------------------------------------------------
    /// set jac_sparsity_
    void set_jac_sparsity(void);
    // -----------------------------------------------------------------------
    // atomic_three virtual functions
    // ------------------------------------------------------------------------
    virtual bool type(
        const vector<Base>&          parameter_x ,
        const vector<ad_type_enum>&  type_x      ,
        vector<ad_type_enum>&        type_y
    );
    virtual bool forward(
        size_t                       need_y     ,
        size_t                       order_low  ,
        size_t                       order_up   ,
        const vector<ad_type_enum>&  type_x     ,
        const vector<Base>&          taylor_x   ,
        vector<Base>&                taylor_y
    );
public:
    // BEGIN_CTOR_PROTOTYPE
    chkpoint_two(
        const std::string& name   ,
        const ADFun<Base>& ad_fun
    );
    // END_CTOR_PROTOTYPE
    //
    // destructor
    ~chkpoint_two(void);
    //
    // BEGIN_USE_PROTOTYPE
    template <class ADVector>
    void operator()(const ADVector& ax, ADVector& ay);
    // END_USE_PROTOTYPE
    //
    // BEGIN_NEW_DYNAMIC_PROTOTYPE
    template <class BaseVector>
    void new_dynamic(const BaseVector& dynamic);
    // END_NEW_DYNAMIC_PROTOTYPE
};

} // END_CPPAD_NAMESPACE


# endif
