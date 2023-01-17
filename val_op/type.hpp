# ifndef TYPE_HPP
# define TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// -------------------------------------------------------------
# include <cassert>
# include <cstddef>
# include <cppad/utility/vector.hpp>

// addr_type
typedef int addr_t;

// Vector
template <class Value> using Vector = CppAD::vector<Value>;

// op_enum_t
enum op_enum_t {
   add_op_enum,
   con_op_enum,
   fun_op_enum,
   sub_op_enum,
   number_op_enum
};

# endif
