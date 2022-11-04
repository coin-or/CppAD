# ifndef CPPAD_LOCAL_ATOMIC_INDEX_HPP
# define CPPAD_LOCAL_ATOMIC_INDEX_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*!
$begin atomic_index$$
$spell
    ptr
    Cpp
$$

$section Store and Retrieve Atomic Function Information by Index$$

$head Syntax$$
$icode%index_out% = local::atomic_index<%Base%>(
    %set_null%, %index_in%, %type%, %name%, %ptr%
)%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_ATOMIC_INDEX%// END_PROTOTYPE%1
%$$

$head Base$$
Is the base type for the tape for the atomic functions
that we are using an index to identify.

$head Get Number Case$$
The get number case is defined by
$icode set_null$$ is true and $icode index_in$$ is zero.
For this case, $icode index_out$$ is set to
the number of atomic functions stored in $codei%atomic_index<%Base%>%$$
and no information is stored or changed.
In this case, the atomic functions correspond to $icode index_in$$ from
one to $icode index_out$$ inclusive.

$head set_null$$
If $icode set_null$$ is true and $icode index_in$$ is zero,
this argument is just used to signal the get number case.
Otherwise, $icode set_null$$
should only be true during a call to an atomic function destructor.
In this case, the $icode ptr$$ corresponding to $icode index_in$$
is set to null
(so that CppAD knows the corresponding atomic function no longer works).

$head index_in$$
If $icode index_in$$ is zero and $icode set_null$$ is true,
this argument is just used to signal the get number case.
Otherwise, see below:


$subhead zero$$
The value $icode index_in$$ should only be zero
during a call to an atomic function constructor.
In this case, a copy of the input value of
$icode type$$, $codei%*%name%$$, and $icode ptr$$ are stored.
The value $icode index_out$$
is the $icode index_in$$ value corresponding to these input values.

$subhead non-zero$$
If $icode index_in$$ is non-zero,
the information corresponding to this index is returned.

$head type$$
This argument is not used in the get number case.
Otherwise if $icode index_in$$ is zero, $icode type$$ is an input.
Otherwise it is set to the value corresponding to $icode index_in$$.
The type corresponding to an index is intended to be
2 for $cref atomic_two$$ functions,
3 for $cref atomic_three$$ functions, and
4 for $cref atomic_four$$ functions,

$head name$$
This argument is not used in the get number case.
Otherwise if $icode index_in$$ is zero,
$icode name$$ is an input and must not be null.
Otherwise, if $icode name$$ is not null, $codei%*%name%$$
is set to the name corresponding to $icode index_in$$.
Allowing for $icode name$$ to be null avoids
a string copy when it is not needed.

$head ptr$$
This argument is not used in the get number case.
Otherwise if $icode index_in$$ is zero, $icode ptr$$ is an input.
Otherwise it is set to the value corresponding to $icode index_in$$.
In the special case where $icode set_null$$ is true,
$icode ptr$$ is set to the null pointer and this is the $icode ptr$$ value
corresponding to $icode index_in$$ for future calls to $code atomic_index$$.

$head index_out$$
In the get number case, this is the number of atomic functions.
Otherwise if $icode index_in$$ is zero,
$icode index_out$$ is non-zero and is the $icode index_in$$ value
corresponding to the input values for
$icode type$$, $codei%*%name%$$, and $icode ptr$$.
Otherwise, $index_out$$ is zero.

$end
*/
# include <vector>
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE

struct atomic_index_info {
    size_t      type;
    std::string name;
    void*       ptr;
};

// BEGIN_ATOMIC_INDEX
template <class Base>
size_t atomic_index(
    bool               set_null      ,
    const size_t&      index_in      ,
    size_t&            type          ,
    std::string*       name          ,
    void*&             ptr           )
// END_PROTOTYPE
{   //
    // information for each index
    static std::vector<atomic_index_info> vec;
# ifndef NDEBUG
    if( index_in == 0 || set_null )
    {   CPPAD_ASSERT_KNOWN( ! thread_alloc::in_parallel(),
        "calling atomic function constructor or destructor in parallel mode"
        );
    }
# endif
    if( set_null & (index_in == 0) )
        return vec.size();
    //
    // case were we are retreving informaiton for an atomic function
    if( 0 < index_in )
    {   CPPAD_ASSERT_UNKNOWN( index_in <= vec.size() )
        //
        // case where we are setting the pointer to null
        if( set_null )
            vec[index_in-1].ptr = nullptr;
        //
        atomic_index_info& entry = vec[index_in - 1];
        type = entry.type;
        ptr  = entry.ptr;
        if( name != nullptr )
            *name  = entry.name;
        return 0;
    }
    //
    // case where we are storing information for an atomic function
    atomic_index_info entry;
    entry.type = type;
    entry.name = *name;
    entry.ptr  = ptr;
    vec.push_back(entry);
    //
    return vec.size();
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
