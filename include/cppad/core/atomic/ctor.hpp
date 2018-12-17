# ifndef CPPAD_CORE_ATOMIC_CTOR_HPP
# define CPPAD_CORE_ATOMIC_CTOR_HPP
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
$begin atomic_ctor$$
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
$icode%atomic_user afun%(%ctor_arg_list%)
%$$
$codei%atomic_base<%Base%>(%name%, %sparsity%)
%$$

$head atomic_user$$

$subhead ctor_arg_list$$
Is a list of arguments for the $icode atomic_user$$ constructor.

$subhead afun$$
The object $icode afun$$ must stay in scope for as long
as the corresponding atomic function is used.
This includes use by any $cref/ADFun<Base>/ADFun/$$ that
has this $icode atomic_user$$ operation in its
$cref/operation sequence/glossary/Operation/Sequence/$$.

$subhead Implementation$$
The user defined $icode atomic_user$$ class is a publicly derived class of
$codei%atomic_base<%Base%>%$$.
It should be declared as follows:
$codei%
    class %atomic_user% : public CppAD::atomic_base<%Base%> {
    public:
        %atomic_user%(%ctor_arg_list%) : atomic_base<%Base%>(%name%, %sparsity%)
    %...%
    };
%$$
where $icode ...$$
denotes the rest of the implementation of the derived class.
This includes completing the constructor and
all the virtual functions that have their
$code atomic_base$$ implementations replaced by
$icode atomic_user$$ implementations.

$head atomic_base$$

$subhead Restrictions$$
The $code atomic_base$$ constructor cannot be called in
$cref/parallel/ta_in_parallel/$$ mode.

$subhead Base$$
The template parameter determines the
$icode Base$$ type for this $codei%AD<%Base%>%$$ atomic operation.

$subhead name$$
This $code atomic_base$$ constructor argument has the following prototype
$codei%
    const std::string& %name%
%$$
It is the name for this atomic function and is used for error reporting.
The suggested value for $icode name$$ is $icode afun$$ or $icode atomic_user$$,
i.e., the name of the corresponding atomic object or class.

$subhead sparsity$$
This $code atomic_base$$ constructor argument has prototype
$codei%
    atomic_base<%Base%>::option_enum %sparsity%
%$$
The current $icode sparsity$$ for an $code atomic_base$$ object
determines which type of sparsity patterns it uses
and its value is one of the following:
$table
$icode sparsity$$   $cnext sparsity patterns $rnext
$codei%atomic_base<%Base%>::pack_sparsity_enum%$$ $pre  $$ $cnext
    $cref/vectorBool/CppAD_vector/vectorBool/$$
$rnext
$codei%atomic_base<%Base%>::bool_sparsity_enum%$$ $pre  $$ $cnext
    $cref/vector/CppAD_vector/$$$code <bool>$$
$rnext
$codei%atomic_base<%Base%>::set_sparsity_enum%$$ $pre  $$ $cnext
    $cref/vector/CppAD_vector/$$$code <std::set<std::size_t> >$$
$tend
There is a default value for $icode sparsity$$ if it is not
included in the constructor (which may be either the bool or set option).

$head Example$$

$subhead Define Constructor$$
The following is an example of a atomic function constructor definitions:
$cref%get_started.cpp%atomic_get_started.cpp%Constructor%$$.

$subhead Use Constructor$$
The following is an example using a atomic function constructor:
$cref%get_started.cpp%atomic_get_started.cpp%Use Atomic Function%Constructor%$$.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/ctor.hpp
Constructors for atomic_base class.
*/

/*!
Base class for atomic_atomic functions.

\tparam Base
This class is used for defining an AD<Base> atomic operation y = f(x).

\par
make sure user does not invoke the default constructor
*/
template <class Base>
atomic_base<Base>::atomic_base(void)
{   CPPAD_ASSERT_KNOWN(false,
        "Attempt to use the atomic_base default constructor"
    );
}
/*!
Constructor

\param name
name used for error reporting

\param sparsity [in]
what type of sparsity patterns are computed by this function,
bool_sparsity_enum or set_sparsity_enum. Default value is
bool sparsity patterns.
*/
template <class Base>
atomic_base<Base>::atomic_base(
        const std::string&     name,
        option_enum            sparsity
) :
index_   ( class_object().size()  )  ,
sparsity_( sparsity               )
{   CPPAD_ASSERT_KNOWN(
        ! thread_alloc::in_parallel() ,
        "atomic_base: constructor cannot be called in parallel mode."
    );
    class_object().push_back(this);
    class_name().push_back(name);
    CPPAD_ASSERT_UNKNOWN( class_object().size() == class_name().size() );
    //
    // initialize work pointers as null;
    for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
        work_[thread] = CPPAD_NULL;
}

} // END_CPPAD_NAMESPACE
# endif
