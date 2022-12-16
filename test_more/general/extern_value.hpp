# ifndef CPPAD_TEST_MORE_GENERAL_EXTERN_VALUE_HPP
# define CPPAD_TEST_MORE_GENERAL_EXTERN_VALUE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

template <class Type>
class extern_value  {
private:
   Type value_;
public:
   extern_value(Type value);
   void set(Type value);
   Type get(void);
};

# endif
