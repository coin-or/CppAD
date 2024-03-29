// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>

// Returns a pointer to a double that has value 5.0
// and must be freed using thread_alloc::return_memory.
double *release_thread_alloc(void)
{  size_t min_bytes = sizeof(double);
   size_t cap_bytes;
   void*   v_ptr = CppAD::thread_alloc::get_memory(min_bytes, cap_bytes);
   double* d_ptr = reinterpret_cast<double*>(v_ptr);
   *d_ptr        = 5.0;
   return d_ptr;
}

// just use ADFun<double> constructor
void release_adfun_ctor(void)
{
   CppAD::vector< CppAD::AD<double> > ax(1), ay(1);
   ax[0] = 0.;
   CppAD::Independent(ax);
   ay[0] = fabs(ax[0]);
   CppAD::ADFun<double> f(ax, ay);
   return;
}
