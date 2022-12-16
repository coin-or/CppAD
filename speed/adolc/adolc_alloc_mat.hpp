# ifndef CPPAD_SPEED_ADOLC_ADOLC_ALLOC_MAT_HPP
# define CPPAD_SPEED_ADOLC_ADOLC_ALLOC_MAT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
double** adolc_alloc_mat(size_t m, size_t n);
void adolc_free_mat(double** mat);

# endif
