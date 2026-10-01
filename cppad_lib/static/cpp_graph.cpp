// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/core/graph/cpp_graph.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*
-------------------------------------------------------------------------------
{xrst_begin cpp_graph_ctor}

C++ AD Graph Constructor
########################

Syntax
******
| ``cpp_graph`` *graph_obj*
| *graph_obj* . ``initialize`` ()

function_name
*************
:ref:`cpp_ad_graph@function_name`
is initialized to the empty string.

n_dynamic_ind
*************
:ref:`cpp_ad_graph@n_dynamic_ind` is initialized as zero.

n_variable_ind
**************
:ref:`cpp_ad_graph@n_variable_ind` is initialized as zero.

constant_vec
************
:ref:`cpp_ad_graph@constant_vec` is initialized as empty.

operator_vec
************
:ref:`cpp_ad_graph@operator_vec` is initialized as empty.

operator_arg
************
:ref:`cpp_ad_graph@operator_arg` is initialized as empty.

dependent_vec
*************
:ref:`cpp_ad_graph@dependent_vec` is initialized as empty.

Parallel Mode
*************
The first use of the ``cpp_graph`` constructor
cannot be in :ref:`parallel<ta_in_parallel-name>` execution mode.

{xrst_end cpp_graph_ctor}
--------------------------------------------------------------------------------
*/
void cpp_graph::initialize(void)
{  function_name_  = "";
    n_dynamic_ind_  = 0;
    n_variable_ind_  = 0;
    discrete_name_vec_.resize(0);
    atomic_name_vec_.resize(0);
    print_text_vec_.resize(0);
    constant_vec_.resize(0);
    operator_vec_.resize(0);
    operator_arg_.resize(0);
    dependent_vec_.resize(0);
    return;
}
cpp_graph::cpp_graph(void)
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
    static bool first = true;
    if( first )
    {  first = false;
        CPPAD_ASSERT_UNKNOWN( local::graph::op_name2enum.size() == 0 );
        // initialize cpp_graph global variables in cpp_graph_op.cpp
        local::graph::set_operator_info();
    }
    initialize();
}

} // END_CPPAD_NAMESPACE
