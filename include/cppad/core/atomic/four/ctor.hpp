# ifndef CPPAD_CORE_ATOMIC_FOUR_CTOR_HPP
# define CPPAD_CORE_ATOMIC_FOUR_CTOR_HPP
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
$begin atomic_four_ctor$$
$spell
    enum
    sq
    std
    afun
    arg
    CppAD
    bool
    ctor
    const
    mat_mul_xam.cpp
    hpp
$$

$section Atomic Function Constructor$$

$head Syntax$$
$codei%class %atomic_user% : public CppAD::atomic_four<%Base%> {
public:
    %atomic_user%(%ctor_arg_list%) : CppAD::atomic_four<%Base%>(%name%)
    %...%
};
%$$
$icode%atomic_user afun%(%ctor_arg_list%)
%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head atomic_user$$

$subhead ctor_arg_list$$
Is a list of arguments for the $icode atomic_user$$ constructor.

$subhead afun$$
The object $icode afun$$ must stay in scope for as long
as the corresponding atomic function is used.
This includes use by any $cref/ADFun<Base>/ADFun/$$ object that
has this $icode atomic_user$$ operation in its
$cref/operation sequence/glossary/Operation/Sequence/$$.

$subhead Implementation$$
The user defined $icode atomic_user$$ class is a publicly derived class of
$codei%atomic_four<%Base%>%$$.
It should be declared as follows:
$codei%
    class %atomic_user% : public CppAD::atomic_four<%Base%> {
    public:
        %atomic_user%(%ctor_arg_list%) : atomic_four<%Base%>(%name%)
    %...%
    };
%$$
where $icode ...$$
denotes the rest of the implementation of the derived class.
This includes completing the constructor and
all the virtual functions that have their
$code atomic_four$$ implementations replaced by
$icode atomic_user$$ implementations.

$head atomic_four$$

$subhead Restrictions$$
The $code atomic_four$$ constructor and destructor cannot be called in
$cref/parallel/ta_in_parallel/$$ mode.

$subhead Base$$
The template parameter determines the
$cref/Base/atomic_four_call/Base/$$
type for this $codei%AD<%Base%>%$$ atomic operation.

$subhead name$$
This $code atomic_four$$ constructor argument has the following prototype
$codei%
    const std::string& %name%
%$$
It is the name for this atomic function and is used for error reporting.
The suggested value for $icode name$$ is $icode afun$$ or $icode atomic_user$$,
i.e., the name of the corresponding atomic object or class.

$head Example$$
The following is an example constructor definition taken from
$cref atomic_four_norm_sq.cpp$$:
$srcfile%
    example/atomic_four/norm_sq.cpp%
    0%// BEGIN CONSTRUCTOR%// END CONSTRUCTOR%0
%$$


$end
-------------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// atomic_four()
template <class Base>
atomic_four<Base>::atomic_four(void)
{   CPPAD_ASSERT_KNOWN(false,
        "Attempt to use the atomic_four default constructor"
    );
}

// atomic_four(name)
// BEGIN_PROTOTYPE
template <class Base>
atomic_four<Base>::atomic_four(const std::string& name )
// END_PROTOTYPE
{   CPPAD_ASSERT_KNOWN(
        ! thread_alloc::in_parallel() ,
        "atomic_four: constructor cannot be called in parallel mode."
    );
    //
    // index_
    bool        set_null  = false;
    size_t      index     = 0;
    size_t      type      = 4;
    std::string copy_name = name;
    void*       copy_this = reinterpret_cast<void*>( this );
    index_  = local::atomic_index<Base>(
        set_null, index, type, &copy_name, copy_this
    );
    //
    // work_
    for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
        work_[thread] = nullptr;
}

} // END_CPPAD_NAMESPACE
# endif
