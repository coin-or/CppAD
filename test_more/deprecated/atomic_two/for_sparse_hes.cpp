// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
@begin atomic_two_for_sparse_hes.cpp@@

$section Atomic Forward Hessian Sparsity: Example and Test$$

$head Purpose$$
This example demonstrates calculation of the forward Hessian sparsity pattern
for an atomic operation.

$head function$$
For this example, the atomic function
$latex f : \B{R}^3 \rightarrow \B{R}^2$$ is defined by
$latex \[
f( x ) = \left( \begin{array}{c}
   x_2 * x_2 \\
   x_0 * x_1
\end{array} \right)
\] $$
The Hessians of the component functions are
$latex \[
f_0^{(2)} ( x ) = \left( \begin{array}{ccc}
   0 & 0 & 0  \\
   0 & 0 & 0  \\
   0 & 0 & 2
\end{array} \right)
\W{,}
f_1^{(2)} ( x ) = \left( \begin{array}{ccc}
   0 & 1 & 0 \\
   1 & 0 & 0 \\
   0 & 0 & 0
\end{array} \right)
\] $$

$head Start  Class Definition$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
namespace {          // isolate items below to this file
using CppAD::vector; // abbreviate as vector
//
class atomic_for_sparse_hes : public CppAD::atomic_base<double> {
/* %$$
$head Constructor $$
$srccode%cpp% */
public:
   // constructor (could use const char* for name)
   atomic_for_sparse_hes(const std::string& name) :
   // this example only uses pack sparsity patterns
   CppAD::atomic_base<double>(name, pack_sparsity_enum)
   { }
private:
/* %$$
$head forward$$
$srccode%cpp% */
   // forward mode routine called by CppAD
   virtual bool forward(
      size_t                    p ,
      size_t                    q ,
      const vector<bool>&      vx ,
      vector<bool>&            vy ,
      const vector<double>&    tx ,
      vector<double>&          ty
   )
   {
# ifndef NDEBUG
      size_t n = tx.size() / (q + 1);
      size_t m = ty.size() / (q + 1);
# endif
      assert( n == 3 );
      assert( m == 2 );

      // return flag
      bool ok = q == 0;
      if( ! ok )
         return ok;

      // check for defining variable information
      // This case must always be implemented
      if( vx.size() > 0 )
      {  vy[0] = vx[0];
         vy[1] = vx[0] || vy[0];
      }

      // Order zero forward mode.
      // This case must always be implemented
      // f(x) = [ x_0 * x_0 ]
      //        [ x_0 * x_1 ]
      assert( p <= 0 );
      if( p <= 0 )
      {  ty[0] = tx[2] * tx[2];
         ty[1] = tx[0] * tx[1];
      }
      return ok;
   }
/* %$$
$head for_sparse_jac$$
$srccode%cpp% */
   // forward Jacobian sparsity routine called by CppAD
   virtual bool for_sparse_jac(
      size_t                     q ,
      const CppAD::vectorBool&   r ,
      CppAD::vectorBool&         s ,
      const vector<double>&      x )
   {  // This function needed because we are using ForSparseHes
      // with afun.option( CppAD::atomic_base<double>::pack_sparsity_enum )
# ifndef NDEBUG
      size_t n = r.size() / q;
      size_t m = s.size() / q;
# endif
      assert( x.size() == n );
      assert( n == 3 );
      assert( m == 2 );


      // f'(x) = [   0,   0, 2 x_2 ]
      //         [ x_1, x_0,     0 ]

      // sparsity for first row of S(x) = f'(x) * R
      size_t i = 0;
      for(size_t j = 0; j < q; j++)
         s[ i * q + j ] = r[ 2 * q + j ];

      // sparsity for second row of S(x) = f'(x) * R
      i = 1;
      for(size_t j = 0; j < q; j++)
         s[ i * q + j ] = r[ 0 * q + j ] || r[ 1 * q + j];

      return true;
   }
/* %$$
$head rev_sparse_jac$$
$srccode%cpp% */
   // reverse Jacobian sparsity routine called by CppAD
   virtual bool rev_sparse_jac(
      size_t                     q  ,
      const CppAD::vectorBool&   rt ,
      CppAD::vectorBool&         st ,
      const vector<double>&      x  )
   {  // This function needed because we are using ForSparseHes
      // with afun.option( CppAD::atomic_base<double>::pack_sparsity_enum )
# ifndef NDEBUG
      size_t m = rt.size() / q;
      size_t n = st.size() / q;
# endif
      assert( x.size() == n );
      assert( n == 3 );
      assert( m == 2 );

      //           [     0,  x_1 ]
      // f'(x)^T = [     0,  x_0 ]
      //           [ 2 x_2,    0 ]

      // sparsity for first row of S(x)^T = f'(x)^T * R^T
      size_t i = 0;
      for(size_t j = 0; j < q; j++)
         st[ i * q + j ] = rt[ 1 * q + j ];

      // sparsity for second row of S(x)^T = f'(x)^T * R^T
      i = 1;
      for(size_t j = 0; j < q; j++)
         st[ i * q + j ] = rt[ 1 * q + j ];

      // sparsity for third row of S(x)^T = f'(x)^T * R^T
      i = 2;
      for(size_t j = 0; j < q; j++)
         st[ i * q + j ] = rt[ 0 * q + j ];

      return true;
   }
/* %$$
$head for_sparse_hes$$
$srccode%cpp% */
   // forward Hessian sparsity routine called by CppAD
   virtual bool for_sparse_hes(
      const vector<bool>&   vx,
      const vector<bool>&   r ,
      const vector<bool>&   s ,
      CppAD::vectorBool&    h ,
      const vector<double>& x )
   {  // This function needed because we are using RevSparseHes
      // with afun.option( CppAD::atomic_base<double>::pack_sparsity_enum )
      size_t n = r.size();
# ifndef NDEBUG
      size_t m = s.size();
# endif
      assert( x.size() == n );
      assert( n == 3 );
      assert( m == 2 );
      assert( h.size() == n * n );

      //            [ 0 , 0 , 0 ]                  [ 0 , 1 , 0 ]
      // f_0''(x) = [ 0 , 0 , 0 ]  f_1^{(2)} (x) = [ 1 , 0 , 0 ]
      //            [ 0 , 0 , 2 ]                  [ 0 , 0 , 0 ]

      // initial entire matrix as false
      for(size_t i = 0; i < n * n; i++)
         h[i] = false;

      // component (2, 2)
      h[ 2 * n + 2 ] = s[0] && r[2];

      // components (1, 0) and (0, 1)
      h[ 1 * n + 0 ] = s[1] && r[0] && r[1];
      h[ 0 * n + 1 ] = s[1] && r[0] && r[1];

      return true;
   }
}; // End of atomic_for_sparse_hes class

/* %$$
$head Use Atomic Function$$
$srccode%cpp% */
bool use_atomic_for_sparse_hes(bool x_1_variable)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();
   //
   // Create the atomic for_sparse_hes object
   atomic_for_sparse_hes afun("atomic_for_sparse_hes");
   //
   // Create the function f(u)
   //
   // domain space vector
   size_t n  = 3;
   double x_0 = 1.00;
   double x_1 = 2.00;
   double x_2 = 3.00;
   vector< AD<double> > au(n);
   au[0] = x_0;
   au[1] = x_1;
   au[2] = x_2;

   // declare independent variables and start tape recording
   CppAD::Independent(au);

   // range space vector
   size_t m = 2;
   vector< AD<double> > ay(m);

   // call atomic function
   vector< AD<double> > ax(n);
   ax[0] = au[0];
   ax[2] = au[2];
   if( x_1_variable )
      ax[1] = au[1];
   else
      ax[1] = x_1;
   afun(ax, ay);          // y = [ x_2 * x_2 ,  x_0 * x_1 ]^T

   // create f: u -> y and stop tape recording
   CppAD::ADFun<double> f;
   f.Dependent (au, ay);  // f(u) = y
   //
   // check function value
   double check = x_2 * x_2;
   ok &= NearEqual( Value(ay[0]) , check,  eps, eps);
   check = x_0 * x_1;
   ok &= NearEqual( Value(ay[1]) , check,  eps, eps);

   // check zero order forward mode
   size_t q;
   vector<double> xq(n), yq(m);
   q     = 0;
   xq[0] = x_0;
   xq[1] = x_1;
   xq[2] = x_2;
   yq    = f.Forward(q, xq);
   check = x_2 * x_2;
   ok &= NearEqual(yq[0] , check,  eps, eps);
   check = x_0 * x_1;
   ok &= NearEqual(yq[1] , check,  eps, eps);

   // forward sparse Hessian
   CppAD::vectorBool r(n), s(m), h(n * n);
   for(size_t j = 0; j < n; j++)
      r[j] = true;
   for(size_t i = 0; i < m; i++)
      s[i] = true;
   h = f.ForSparseHes(r, s);

   // check result
   CppAD::vectorBool check_h(n * n);
   for(size_t i = 0; i < n * n; i++)
      check_h[i] = false;
   check_h[ 2 * n + 2 ] = true;
   if( x_1_variable )
   {  check_h[0 * n + 1] = true;
      check_h[1 * n + 0] = true;
   }
   for(size_t i = 0; i < n * n; i++)
      ok &= h[ i ] == check_h[ i ];
   //
   return ok;
}
}  // End empty namespace
/* %$$
$head Test with x_1 Both a Variable and a Parameter$$
$srccode%cpp% */
bool for_sparse_hes(void)
{  bool ok = true;
   // test with x_1 a variable
   ok     &= use_atomic_for_sparse_hes(true);
   // test with x_1 a parameter
   ok     &= use_atomic_for_sparse_hes(false);
   return ok;
}
/* %$$
$end
*/
