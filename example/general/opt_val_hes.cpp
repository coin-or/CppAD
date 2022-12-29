// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin opt_val_hes.cpp app}

opt_val_hes: Example and Test
#############################

Fix :math:`z \in \B{R}^\ell` and define the functions
:math:`S_k : \B{R} \times \B{R} \rightarrow \B{R}^\ell` by and
:math:`F : \B{R} \times \B{R} \rightarrow \B{R}` by

.. math::
   :nowrap:

   \begin{eqnarray}
   S_k (x, y) & = & \frac{1}{2} [ y * \sin ( x * t_k ) - z_k ]^2
   \\
   F(x, y)    & = &  \sum_{k=0}^{\ell-1} S_k (x, y)
   \end{eqnarray}

It follows that

.. math::
   :nowrap:

   \begin{eqnarray}
   \partial_y F(x, y)
   & = &
   \sum_{k=0}^{\ell-1} [ y * \sin ( x * t_k ) - z_k ] \sin( x * t_k )
   \\
   \partial_y \partial_y F(x, y)
   & = &
   \sum_{k=0}^{\ell-1} \sin ( x t_k )^2
   \end{eqnarray}

Furthermore if we define :math:`Y(x)`
as solving the equation :math:`\partial F[ x, Y(x) ] = 0` we have

.. math::
   :nowrap:

   \begin{eqnarray}
   0 & = &
   \sum_{k=0}^{\ell-1} [ Y(x) * \sin ( x * t_k ) - z_k ] \sin( x * t_k )
   \\
   Y(x) \sum_{k=0}^{\ell-1} \sin ( x * t_k )^2
   - \sum_{k=0}^{\ell-1} \sin ( x * t_k ) z_k
   \\
   Y(x) & = & \frac{
      \sum_{k=0}^{\ell-1} \sin( x * t_k ) z_k
      }{
      \sum_{k=0}^{\ell-1} \sin ( x * t_k )^2
   }
   \end{eqnarray}

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end opt_val_hes.cpp}
*/
// BEGIN C++

# include <limits>
# include <cppad/cppad.hpp>

namespace {
   using CppAD::AD;
   typedef CPPAD_TESTVECTOR(double)       BaseVector;
   typedef CPPAD_TESTVECTOR(AD<double>) ADVector;

   class Fun {
   private:
      const BaseVector t_;    // measurement times
      const BaseVector z_;    // measurement values
   public:
      typedef ADVector ad_vector;
      // constructor
      Fun(const BaseVector &t, const BaseVector &z)
      : t_(t) , z_(z)
      {  assert( t.size() == z.size() ); }
      // ell
      size_t ell(void) const
      {  return t_.size(); }
      // Fun.s
      AD<double> s(size_t k, const ad_vector& x, const ad_vector& y) const
      {
         AD<double> residual = y[0] * sin( x[0] * t_[k] ) - z_[k];
         AD<double> s_k      = .5 * residual * residual;

         return s_k;
      }
      // Fun.sy
      ad_vector sy(size_t k, const ad_vector& x, const ad_vector& y) const
      {  assert( y.size() == 1);
         ad_vector sy_k(1);

         AD<double> residual = y[0] * sin( x[0] * t_[k] ) - z_[k];
         sy_k[0] = residual * sin( x[0] * t_[k] );

         return sy_k;
      }
   };
   // Used to test calculation of Hessian of V
   AD<double> V(const ADVector& x, const BaseVector& t, const BaseVector& z)
   {  // compute Y(x)
      AD<double> numerator = 0.;
      AD<double> denominator = 0.;
      size_t k;
      for(k = 0; k < size_t(t.size()); k++)
      {  numerator   += sin( x[0] * t[k] ) * z[k];
         denominator += sin( x[0] * t[k] ) * sin( x[0] * t[k] );
      }
      AD<double> y = numerator / denominator;

      // V(x) = F[x, Y(x)]
      AD<double> sum = 0;
      for(k = 0; k < size_t(t.size()); k++)
      {  AD<double> residual = y * sin( x[0] * t[k] ) - z[k];
         sum += .5 * residual * residual;
      }
      return sum;
   }
}

bool opt_val_hes(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;

   // temporary indices
   size_t j, k;

   // x space vector
   size_t n = 1;
   BaseVector x(n);
   x[0] = 2. * 3.141592653;

   // y space vector
   size_t m = 1;
   BaseVector y(m);
   y[0] = 1.;

   // t and z vectors
   size_t ell = 10;
   BaseVector t(ell);
   BaseVector z(ell);
   for(k = 0; k < ell; k++)
   {  t[k] = double(k) / double(ell);       // time of measurement
      z[k] = y[0] * sin( x[0] * t[k] );     // data without noise
   }

   // construct the function object
   Fun fun(t, z);

   // evaluate the Jacobian and Hessian
   BaseVector jac(n), hes(n * n);
# ifndef NDEBUG
   int signdet =
# endif
   CppAD::opt_val_hes(x, y, fun, jac, hes);

   // we know that F_yy is positive definate for this case
   assert( signdet == 1 );

   // create ADFun object g corresponding to V(x)
   ADVector a_x(n), a_v(1);
   for(j = 0; j < n; j++)
      a_x[j] = x[j];
   Independent(a_x);
   a_v[0] = V(a_x, t, z);
   CppAD::ADFun<double> g(a_x, a_v);

   // accuracy for checks
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();

   // check Jacobian
   BaseVector check_jac = g.Jacobian(x);
   for(j = 0; j < n; j++)
      ok &= NearEqual(jac[j], check_jac[j], eps, eps);

   // check Hessian
   BaseVector check_hes = g.Hessian(x, 0);
   for(j = 0; j < n*n; j++)
      ok &= NearEqual(hes[j], check_hes[j], eps, eps);

   return ok;
}

// END C++
