// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sub_sparse_hes.cpp}
{xrst_spell
  nv
}

Computing Sparse Hessian for a Subset of Variables
##################################################

Purpose
*******
This example uses
:ref:`multiple levels of AD<mul_level-name>`
to compute the Hessian for a subset of the variables
without having to compute the sparsity pattern for the entire function.

See Also
********
:ref:`sparse_sub_hes.cpp-name` , :ref:`sparsity_sub.cpp-name` ,

Function
********
We consider the function
:math:`f : \B{R}^{nu} \times \B{R}^{nv}  \rightarrow \B{R}` defined by

.. math::

   f (u, v) =
   \left( \sum_{j=0}^{nu-1} u_j^3 \right)
   \left( \sum_{j=0}^{nv-1} v_j \right)

Subset
******
Suppose that we are only interested computing the function

.. math::

   H(u, v) = \partial_u \partial_u f (u, v)

where this Hessian is sparse.

Example
*******
The following code shows one way to compute this subset of the
Hessian of :math:`f`.
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sub_sparse_hes.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace {
   using CppAD::vector;
   template <class Scalar>
   Scalar f(const vector<Scalar>& u,const vector<Scalar>& v)
   {  size_t i;
      Scalar sum_v = Scalar(0);
      for(i = 0; i < v.size(); i++)
         sum_v += v[i];
      Scalar sum_cube_u = Scalar(0);
      for(i = 0; i < u.size(); i++)
         sum_cube_u += u[i] * u[i] * u[i] / 6.0;
      return sum_v * sum_cube_u;
   }
}

bool sub_sparse_hes(void)
{  bool ok = true;
   using CppAD::AD;
   typedef AD<double>   adouble;
   typedef AD<adouble> a2double;
   typedef vector< std::set<size_t> > pattern;
   double eps = 10. * std::numeric_limits<double>::epsilon();
   size_t i, j;

   // start recording with x = (u , v)
   size_t nu = 10;
   size_t nv = 5;
   size_t n  = nu + nv;
   vector<adouble> ax(n);
   for(j = 0; j < n; j++)
      ax[j] = adouble(j + 2);
   CppAD::Independent(ax);

   // extract u as independent variables
   vector<a2double> a2u(nu);
   for(j = 0; j < nu; j++)
      a2u[j] = a2double(j + 2);
   CppAD::Independent(a2u);

   // extract v as parameters
   vector<a2double> a2v(nv);
   for(j = 0; j < nv; j++)
      a2v[j] = ax[nu+j];

   // record g(u)
   vector<a2double> a2y(1);
   a2y[0] = f(a2u, a2v);
   CppAD::ADFun<adouble> g;
   g.Dependent(a2u, a2y);

   // compue sparsity pattern for Hessian of g(u)
   pattern r(nu), s(1);
   for(j = 0; j < nu; j++)
      r[j].insert(j);
   g.ForSparseJac(nu, r);
   s[0].insert(0);
   pattern p = g.RevSparseHes(nu, s);

   // Row and column indices for non-zeros in lower triangle of Hessian
   vector<size_t> row, col;
   for(i = 0; i < nu; i++)
   {  std::set<size_t>::const_iterator itr;
      for(itr = p[i].begin(); itr != p[i].end(); itr++)
      {  j = *itr;
         if( j <= i )
         {  row.push_back(i);
            col.push_back(j);
         }
      }
   }
   size_t K = row.size();
   CppAD::sparse_hessian_work work;
   vector<adouble> au(nu), ahes(K), aw(1);
   aw[0] = 1.0;
   for(j = 0; j < nu; j++)
      au[j] = ax[j];
   size_t n_sweep = g.SparseHessian(au, aw, p, row, col, ahes, work);

   // The Hessian w.r.t u is diagonal
   ok &= n_sweep == 1;

   // record H(u, v) = Hessian of f w.r.t u
   CppAD::ADFun<double> H(ax, ahes);

   // remove unecessary operations
   H.optimize();

   // Now evaluate the Hessian at a particular value for u, v
   vector<double> u(nu), v(nv), x(n);
   for(j = 0; j < n; j++)
      x[j] = double(j + 2);
   vector<double> hes = H.Forward(0, x);

   // Now check the Hessian
   double sum_v = 0.0;
   for(j = 0; j < nv; j++)
      sum_v += x[nu + j];
   for(size_t k = 0; k < K; k++)
   {  i     = row[k];
      j     = col[k];
      ok   &= i == j;
      double check = sum_v * x[i];
      ok &= CppAD::NearEqual(hes[k], check, eps, eps);
   }
   return ok;
}
// END C++
