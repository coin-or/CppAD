# ifndef CPPAD_LOCAL_VAR_OP_ERF_OP_HPP
# define CPPAD_LOCAL_VAR_OP_ERF_OP_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/var_op/mul_op.hpp>
# include <cppad/local/var_op/sub_op.hpp>
# include <cppad/local/var_op/exp_op.hpp>


namespace CppAD { namespace local { namespace var_op {
/*!
\file erf_op.hpp
Forward and reverse mode calculations for z = erf(x) or erfc(x).
*/

/*!
Forward mode Taylor coefficient for result of op = ErfOp or ErfcOp.

The C++ source code corresponding to this operation is one of
\verbatim
   z = erf(x)
   z = erfc(x)
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type Base.

\param op
must be either ErfOp or ErfcOp and indicates if this is
z = erf(x) or z = erfc(x).

\param p
lowest order of the Taylor coefficients that we are computing.

\param q
highest order of the Taylor coefficients that we are computing.

\param i_z
variable index corresponding to the last (primary) result for this operation;
i.e. the row index in taylor corresponding to z.
The auxiliary results are called y_j have index i_z - j.

\param arg
arg[0]: is the variable index corresponding to x.
\n
arg[1]: is the parameter index corresponding to the value zero.
\n
arg[2]: is  the parameter index correspodning to the value 2 / sqrt(pi).

\param parameter
parameter[ arg[1] ] is the value zero,
and parameter[ arg[2] ] is the value 2 / sqrt(pi).

\param cap_order
maximum number of orders that will fit in the taylor array.

\param taylor
\b Input:
taylor [ size_t(arg[0]) * cap_order + k ]
for k = 0 , ... , q,
is the k-th order Taylor coefficient corresponding to x.
\n
\b Input:
taylor [ i_z * cap_order + k ]
for k = 0 , ... , p - 1,
is the k-th order Taylor coefficient corresponding to z.
\n
\b Input:
taylor [ ( i_z - j) * cap_order + k ]
for k = 0 , ... , p-1,
and j = 0 , ... , 4,
is the k-th order Taylor coefficient corresponding to the j-th result for z.
\n
\b Output:
taylor [ (i_z-j) * cap_order + k ],
for k = p , ... , q,
and j = 0 , ... , 4,
is the k-th order Taylor coefficient corresponding to the j-th result for z.

*/
template <class Base>
inline void erf_forward_any(
   op_code_var   op          ,
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 5 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );
   CPPAD_ASSERT_UNKNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >= i_z + 2
   );

   // array used to pass parameter values for sub-operations
   addr_t addr[2];

   // convert from final result to first result
   i_z -= 4; // 4 = NumRes(ErfOp) - 1;

   // z_0 = x * x
   addr[0] = arg[0]; // x
   addr[1] = arg[0]; // x
   mulvv_forward_any(p, q, i_z+0, addr, parameter, cap_order, taylor);

   // z_1 = - x * x
   addr[0] = arg[1];           // zero
   addr[1] = addr_t( i_z );    // z_0
   subpv_forward_any(p, q, i_z+1, addr, parameter, cap_order, taylor);

   // z_2 = exp( - x * x )
   addr[0] = addr_t(i_z+1);
   exp_forward_any(p, q, i_z+2, addr, cap_order, taylor);

   // z_3 = (2 / sqrt(pi)) * exp( - x * x )
   addr[0] = arg[2];            // 2 / sqrt(pi)
   addr[1] = addr_t( i_z + 2 ); // z_2
   mulpv_forward_any(p, q, i_z+3, addr, parameter, cap_order, taylor);

   // pointers to taylor coefficients for x , z_3, and z_4
   Base* x    = taylor + size_t(arg[0]) * cap_order;
   Base* z_3  = taylor + (i_z+3) * cap_order;
   Base* z_4  = taylor + (i_z+4) * cap_order;

   // calculte z_4 coefficients
   if( p == 0 )
   {  // z4 (t) = erf[x(t)]
      if( op == ErfOp )
         z_4[0] = erf(x[0]);
      else
         z_4[0] = erfc(x[0]);
      p++;
   }
   // sign
   Base sign(1.0);
   if( op == ErfcOp )
      sign = Base(-1.0);
   //
   for(size_t j = p; j <= q; j++)
   {  // erf:  z_4' (t) =   erf'[x(t)] * x'(t) = z3(t) * x'(t)
      // erfc: z_4' (t) = - erf'[x(t)] * x'(t) = - z3(t) * x'(t)
      // z_4[1] + 2 * z_4[2] * t +  ... =
      // sign * (z_3[0] + z_3[1] * t +  ...) * (x[1] + 2 * x[2] * t +  ...)
      Base base_j = static_cast<Base>(double(j));
      z_4[j]      = static_cast<Base>(0);
      for(size_t k = 1; k <= j; k++)
         z_4[j] += sign * (Base(double(k)) / base_j) * x[k] * z_3[j-k];
   }
}

/*!
Zero order Forward mode Taylor coefficient for result of op = ErfOp or ErfcOp.

The C++ source code corresponding to this operation one of
\verbatim
   z = erf(x)
   z = erfc(x)
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type Base.

\param op
must be either ErfOp or ErfcOp and indicates if this is
z = erf(x) or z = erfc(x).

\param i_z
variable index corresponding to the last (primary) result for this operation;
i.e. the row index in taylor corresponding to z.
The auxiliary results are called y_j have index i_z - j.

\param arg
arg[0]: is the variable index corresponding to x.
\n
arg[1]: is the parameter index corresponding to the value zero.
\n
arg[2]: is  the parameter index correspodning to the value 2 / sqrt(pi).

\param parameter
parameter[ arg[1] ] is the value zero,
and parameter[ arg[2] ] is the value 2 / sqrt(pi).

\param cap_order
maximum number of orders that will fit in the taylor array.

\param taylor
\b Input:
taylor [ size_t(arg[0]) * cap_order + 0 ]
is the zero order Taylor coefficient corresponding to x.
\n
\b Input:
taylor [ i_z * cap_order + 0 ]
is the zero order Taylor coefficient corresponding to z.
\n
\b Output:
taylor [ (i_z-j) * cap_order + 0 ],
for j = 0 , ... , 4,
is the zero order Taylor coefficient for j-th result corresponding to z.

*/
template <class Base>
inline void erf_forward_0(
   op_code_var   op          ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 5 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );
   CPPAD_ASSERT_UNKNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >= i_z + 2
   );

   // array used to pass parameter values for sub-operations
   addr_t addr[2];

   // convert from final result to first result
   i_z -= 4; // 4 = NumRes(ErfOp) - 1;

   // z_0 = x * x
   addr[0] = arg[0]; // x
   addr[1] = arg[0]; // x
   mulvv_forward_0(i_z+0, addr, parameter, cap_order, taylor);

   // z_1 = - x * x
   addr[0] = arg[1];       // zero
   addr[1] = addr_t(i_z);  // z_0
   subpv_forward_0(i_z+1, addr, parameter, cap_order, taylor);

   // z_2 = exp( - x * x )
   addr[0] = addr_t(i_z+1);
   exp_forward_0(i_z+2, addr, cap_order, taylor);

   // z_3 = (2 / sqrt(pi)) * exp( - x * x )
   addr[0] = arg[2];          // 2 / sqrt(pi)
   addr[1] = addr_t(i_z + 2); // z_2
   mulpv_forward_0(i_z+3, addr, parameter, cap_order, taylor);

   // zero order Taylor coefficient for z_4
   Base* x    = taylor + size_t(arg[0]) * cap_order;
   Base* z_4  = taylor + (i_z + 4) * cap_order;
   if( op == ErfOp )
      z_4[0] = erf(x[0]);
   else
      z_4[0] = erfc(x[0]);
}
/*!
Forward mode Taylor coefficient for result of op = ErfOp or ErfcOp.

The C++ source code corresponding to this operation is one of
\verbatim
   z = erf(x)
   z = erfc(x)
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type Base.

\param op
must be either ErfOp or ErfcOp and indicates if this is
z = erf(x) or z = erfc(x).

\param q
order of the Taylor coefficients that we are computing.

\param r
number of directions for the Taylor coefficients that we afre computing.

\param i_z
variable index corresponding to the last (primary) result for this operation;
i.e. the row index in taylor corresponding to z.
The auxiliary results have index i_z - j for j = 0 , ... , 4
(and include z).

\param arg
arg[0]: is the variable index corresponding to x.
\n
arg[1]: is the parameter index corresponding to the value zero.
\n
arg[2]: is  the parameter index correspodning to the value 2 / sqrt(pi).

\param parameter
parameter[ arg[1] ] is the value zero,
and parameter[ arg[2] ] is the value 2 / sqrt(pi).

\param cap_order
maximum number of orders that will fit in the taylor array.

\par tpv
We use the notation
<code>tpv = (cap_order-1) * r + 1</code>
which is the number of Taylor coefficients per variable

\param taylor
\b Input: If x is a variable,
<code>taylor [ arg[0] * tpv + 0 ]</code>,
is the zero order Taylor coefficient for all directions and
<code>taylor [ arg[0] * tpv + (k-1)*r + ell + 1 ]</code>,
for k = 1 , ... , q,
ell = 0, ..., r-1,
is the k-th order Taylor coefficient
corresponding to x and the ell-th direction.
\n
\b Input:
taylor [ (i_z - j) * tpv + 0 ]
is the zero order Taylor coefficient for all directions and the
j-th result for z.
for k = 1 , ... , q-1,
ell = 0, ... , r-1,
<code>
taylor[ (i_z - j) * tpv + (k-1)*r + ell + 1]
</code>
is the Taylor coefficient for the k-th order, ell-th direction,
and j-th auzillary result.
\n
\b Output:
taylor [ (i_z-j) * tpv + (q-1)*r + ell + 1 ],
for ell = 0 , ... , r-1,
is the Taylor coefficient for the q-th order, ell-th direction,
and j-th auzillary result.

*/
template <class Base>
inline void erf_forward_dir(
   op_code_var   op          ,
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // q, r
   size_t q = order_up;
   size_t r = n_dir;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 5 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >= i_z + 2
   );

   // array used to pass parameter values for sub-operations
   addr_t addr[2];

   // convert from final result to first result
   i_z -= 4; // 4 = NumRes(ErfOp) - 1;

   // z_0 = x * x
   addr[0] = arg[0]; // x
   addr[1] = arg[0]; // x
   mulvv_forward_dir(q, r, i_z+0, addr, parameter, cap_order, taylor);

   // z_1 = - x * x
   addr[0] = arg[1];         // zero
   addr[1] = addr_t( i_z );  // z_0
   subpv_forward_dir(q, r, i_z+1, addr, parameter, cap_order, taylor);

   // z_2 = exp( - x * x )
   addr[0] = addr_t(i_z+1);
   exp_forward_dir(q, r, i_z+2, addr, cap_order, taylor);

   // z_3 = (2 / sqrt(pi)) * exp( - x * x )
   addr[0] = arg[2];            // 2 / sqrt(pi)
   addr[1] = addr_t( i_z + 2 ); // z_2
   mulpv_forward_dir(q, r, i_z+3, addr, parameter, cap_order, taylor);

   // pointers to taylor coefficients for x , z_3, and z_4
   size_t num_taylor_per_var = (cap_order - 1) * r + 1;
   Base* x    = taylor + size_t(arg[0]) * num_taylor_per_var;
   Base* z_3  = taylor + (i_z+3) * num_taylor_per_var;
   Base* z_4  = taylor + (i_z+4) * num_taylor_per_var;

   // sign
   Base sign(1.0);
   if( op == ErfcOp )
      sign = Base(-1.0);

   // erf:  z_4' (t) =   erf'[x(t)] * x'(t) = z3(t) * x'(t)
   // erfc: z_4' (t) = - erf'[x(t)] * x'(t) = z3(t) * x'(t)
   // z_4[1] + 2 * z_4[2] * t +  ... =
   // sign * (z_3[0] + z_3[1] * t +  ...) * (x[1] + 2 * x[2] * t +  ...)
   Base base_q = static_cast<Base>(double(q));
   for(size_t ell = 0; ell < r; ell++)
   {  // index in z_4 and x for q-th order term
      size_t m = (q-1)*r + ell + 1;
      // initialize q-th order term summation
      z_4[m] = sign * z_3[0] * x[m];
      for(size_t k = 1; k < q; k++)
      {  size_t x_index  = (k-1)*r + ell + 1;
         size_t z3_index = (q-k-1)*r + ell + 1;
         Base bk = Base(double(k));
         z_4[m] += sign * (bk / base_q) * x[x_index] * z_3[z3_index];
      }
   }
}

/*!
Compute reverse mode partial derivatives for result of op = ErfOp or ErfcOp.

The C++ source code corresponding to this operation is one of
\verbatim
   z = erf(x)
   z = erfc(x)
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type Base.

\param op
must be either ErfOp or ErfcOp and indicates if this is
z = erf(x) or z = erfc(x).

\param d
highest order Taylor of the Taylor coefficients that we are computing
the partial derivatives with respect to.

\param i_z
variable index corresponding to the last (primary) result for this operation;
i.e. the row index in taylor corresponding to z.
The auxiliary results are called y_j have index i_z - j.

\param arg
arg[0]: is the variable index corresponding to x.
\n
arg[1]: is the parameter index corresponding to the value zero.
\n
arg[2]: is  the parameter index correspodning to the value 2 / sqrt(pi).

\param parameter
parameter[ arg[1] ] is the value zero,
and parameter[ arg[2] ] is the value 2 / sqrt(pi).

\param cap_order
maximum number of orders that will fit in the taylor array.

\param taylor
\b Input:
taylor [ size_t(arg[0]) * cap_order + k ]
for k = 0 , ... , d,
is the k-th order Taylor coefficient corresponding to x.
\n
taylor [ (i_z - j) * cap_order + k ]
for k = 0 , ... , d,
and for j = 0 , ... , 4,
is the k-th order Taylor coefficient corresponding to the j-th result
for this operation.

\param n_order
number of columns in the matrix containing all the partial derivatives

\param partial
\b Input:
partial [ size_t(arg[0]) * n_order + k ]
for k = 0 , ... , d,
is the partial derivative of G( z , x , w , u , ... ) with respect to
the k-th order Taylor coefficient for x.
\n
\b Input:
partial [ (i_z - j) * n_order + k ]
for k = 0 , ... , d,
and for j = 0 , ... , 4,
is the partial derivative of G( z , x , w , u , ... ) with respect to
the k-th order Taylor coefficient for the j-th result of this operation.
\n
\b Output:
partial [ size_t(arg[0]) * n_order + k ]
for k = 0 , ... , d,
is the partial derivative of H( x , w , u , ... ) with respect to
the k-th order Taylor coefficient for x.
\n
\b Output:
partial [ (i_z-j) * n_order + k ]
for k = 0 , ... , d,
and for j = 0 , ... , 4,
may be used as work space; i.e., may change in an unspecified manner.

*/
template <class Base>
inline void erf_reverse(
   op_code_var   op          ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   const Base*   taylor      ,
   size_t        n_order     ,
   Base*         partial     )
{  // d
   //
   size_t d = n_order - 1;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 5 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );
   CPPAD_ASSERT_UNKNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >= i_z + 2
   );

   // array used to pass parameter values for sub-operations
   addr_t addr[2];

   // If pz is zero, make sure this operation has no effect
   // (zero times infinity or nan would be non-zero).
   Base* pz  = partial + i_z * n_order;
   bool skip(true);
   for(size_t i_d = 0; i_d <= d; i_d++)
      skip &= IdenticalZero(pz[i_d]);
   if( skip )
      return;

   // convert from final result to first result
   i_z -= 4; // 4 = NumRes(ErfOp) - 1;

   // Taylor coefficients and partials corresponding to x
   const Base* x  = taylor  + size_t(arg[0]) * cap_order;
   Base* px       = partial + size_t(arg[0]) * n_order;

   // Taylor coefficients and partials corresponding to z_3
   const Base* z_3  = taylor  + (i_z+3) * cap_order;
   Base* pz_3       = partial + (i_z+3) * n_order;

   // Taylor coefficients and partials corresponding to z_4
   Base* pz_4 = partial + (i_z+4) * n_order;

   // sign
   Base sign(1.0);
   if( op == ErfcOp )
      sign = Base(-1.0);

   // Reverse z_4
   size_t j = d;
   while(j)
   {  pz_4[j] /= Base(double(j));
      for(size_t k = 1; k <= j; k++)
      {  px[k]     += sign * azmul(pz_4[j], z_3[j-k]) * Base(double(k));
         pz_3[j-k] += sign * azmul(pz_4[j], x[k]) * Base(double(k));
      }
      j--;
   }
   px[0] += sign * azmul(pz_4[0], z_3[0]);

   // z_3 = (2 / sqrt(pi)) * exp( - x * x )
   addr[0] = arg[2];            // 2 / sqrt(pi)
   addr[1] = addr_t( i_z + 2 ); // z_2
   mulpv_reverse(
      i_z+3, addr, parameter, cap_order, taylor, n_order, partial
   );

   // z_2 = exp( - x * x )
   addr[0] = addr_t(i_z + 1);
   exp_reverse(
      i_z+2, addr, cap_order, taylor, n_order, partial
   );

   // z_1 = - x * x
   addr[0] = arg[1];           // zero
   addr[1] = addr_t( i_z );    // z_0
   subpv_reverse(
      i_z+1, addr, parameter, cap_order, taylor, n_order, partial
   );

   // z_0 = x * x
   addr[0] = arg[0]; // x
   addr[1] = arg[0]; // x
   mulvv_reverse(
      i_z+0, addr, parameter, cap_order, taylor, n_order, partial
   );

}


} } } // END namespace
# endif // CPPAD_ERF_OP_INCLUDED
