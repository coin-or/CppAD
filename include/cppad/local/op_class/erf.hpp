# ifndef CPPAD_LOCAL_OP_CLASS_ERF_HPP
# define CPPAD_LOCAL_OP_CLASS_ERF_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin var_erf dev}
{xrst_spell
   erfc
}

The Error Function and Its Complement
#####################################

Syntax
******
| ``CppAD::local::op_class::erf_v_t`` < *Base* >  *var_erf_op*
| ``CppAD::local::op_class::erfc_v_t`` < *Base* >  *var_erf_op*

Prototype
*********

erf_v
=====
{xrst_literal ,
   include/cppad/local/op_class/erf_v.hpp
   BEGIN NAMESPACE, END NAMESPACE
   BEGIN ERF_V_T , END ERF_V_T
}

erfc_v
======
{xrst_literal ,
   include/cppad/local/op_class/erfc_v.hpp
   BEGIN NAMESPACE, END NAMESPACE
   BEGIN ERFC_V_T , END ERFC_V_T
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by these operators are done using
type *Base* .


op2enum
*******
is the enum value corresponding to this operator; i.e., the inverse of
:ref:`var_enum2op-name` .

n_arg
*****
The number of arguments for the these operators and is always two,
we use *x* to denote the first argument,
the second argument is the constant 2 / sqrt(pi):
{xrst_literal
   include/cppad/local/op_class/erf_v.hpp
   // BEGIN N_ARG
   // END N_ARG
}

n_res
*****
The number of results for the these operators and is always five:
{xrst_literal ,
   include/cppad/local/op_class/erf_v.hpp
   // BEGIN N_RES , // END N_RES
}

.. math::

   z_0 &= x \cdot x
   \\
   z_1 &= - x \cdot x
   \\
   z_2 &= \exp ( - x \cdot x )
   \\
   z_3 &= \exp ( - x \cdot x ) \cdot 2 / \sqrt{\pi}
   \\
   z_3 &= \exp ( - x \cdot x ) \cdot 2 / \sqrt{\pi}

For the error function operator erf_v, the last result is

.. math::

   z_4 = \int_0^x \R{d} t \exp ( - t \cdot t ) \cdot 2 / \sqrt{\pi}

For the complementary error function operator erfc_v ,  the last result is

.. math::

   z_4 = 1 - \int_0^x \R{d} t \exp ( - t \cdot t ) \cdot 2 / \sqrt{\pi}




{xrst_end var_erf}
-----------------------------------------------------------------------------
*/

# include <cppad/local/op/mul_op.hpp>
# include <cppad/local/op/exp_op.hpp>
# include <cppad/local/op/neg_op.hpp>


namespace CppAD { namespace local { namespace op_class { // BEGIN namespace

//
// forward_erf_op
template <class Base>
void forward_erf_op(
   OpCode        op          ,
   size_t        p           ,
   size_t        q           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
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
   forward_mulvv_op(p, q, i_z+0, addr, parameter, cap_order, taylor);

   // z_1 = - x * x
   forward_neg_op(p, q, i_z+1, i_z+0, cap_order, taylor);

   // z_2 = exp( - x * x )
   forward_exp_op(p, q, i_z+2, i_z+1, cap_order, taylor);

   // z_3 = (2 / sqrt(pi)) * exp( - x * x )
   addr[0] = arg[1];            // 2 / sqrt(pi)
   addr[1] = addr_t( i_z + 2 ); // z_2
   forward_mulpv_op(p, q, i_z+3, addr, parameter, cap_order, taylor);

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
//
// forward_erf_op_0
template <class Base>
void forward_erf_op_0(
   OpCode        op          ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
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
   forward_mulvv_op_0(i_z+0, addr, parameter, cap_order, taylor);

   // z_1 = - x * x
   forward_neg_op_0(i_z+1, i_z+0, cap_order, taylor);

   // z_2 = exp( - x * x )
   forward_exp_op_0(i_z+2, i_z+1, cap_order, taylor);

   // z_3 = (2 / sqrt(pi)) * exp( - x * x )
   addr[0] = arg[1];          // 2 / sqrt(pi)
   addr[1] = addr_t(i_z + 2); // z_2
   forward_mulpv_op_0(i_z+3, addr, parameter, cap_order, taylor);

   // zero order Taylor coefficient for z_4
   Base* x    = taylor + size_t(arg[0]) * cap_order;
   Base* z_4  = taylor + (i_z + 4) * cap_order;
   if( op == ErfOp )
      z_4[0] = erf(x[0]);
   else
      z_4[0] = erfc(x[0]);
}
//
// forward_erf_op_dir
template <class Base>
void forward_erf_op_dir(
   OpCode        op          ,
   size_t        q           ,
   size_t        r           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
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
   forward_mulvv_op_dir(q, r, i_z+0, addr, parameter, cap_order, taylor);

   // z_1 = - x * x
   forward_neg_op_dir(q, r, i_z+1, i_z+0, cap_order, taylor);

   // z_2 = exp( - x * x )
   forward_exp_op_dir(q, r, i_z+2, i_z+1, cap_order, taylor);

   // z_3 = (2 / sqrt(pi)) * exp( - x * x )
   addr[0] = arg[1];            // 2 / sqrt(pi)
   addr[1] = addr_t( i_z + 2 ); // z_2
   forward_mulpv_op_dir(q, r, i_z+3, addr, parameter, cap_order, taylor);

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
The auxillary results are called y_j have index i_z - j.

\param arg
arg[0]: is the variable index corresponding to x.
\n
arg[1]: is  the parameter index correspodning to the value 2 / sqrt(pi).

\param parameter
parameter[ arg[1] ] is the value 2 / sqrt(pi).

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

\param nc_partial
number of columns in the matrix containing all the partial derivatives

\param partial
\b Input:
partial [ size_t(arg[0]) * nc_partial + k ]
for k = 0 , ... , d,
is the partial derivative of G( z , x , w , u , ... ) with respect to
the k-th order Taylor coefficient for x.
\n
\b Input:
partial [ (i_z - j) * nc_partial + k ]
for k = 0 , ... , d,
and for j = 0 , ... , 4,
is the partial derivative of G( z , x , w , u , ... ) with respect to
the k-th order Taylor coefficient for the j-th result of this operation.
\n
\b Output:
partial [ size_t(arg[0]) * nc_partial + k ]
for k = 0 , ... , d,
is the partial derivative of H( x , w , u , ... ) with respect to
the k-th order Taylor coefficient for x.
\n
\b Output:
partial [ (i_z-j) * nc_partial + k ]
for k = 0 , ... , d,
and for j = 0 , ... , 4,
may be used as work space; i.e., may change in an unspecified manner.

*/
template <class Base>
void reverse_erf_op(
   OpCode        op          ,
   size_t        d           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   const Base*   taylor      ,
   size_t        nc_partial  ,
   Base*         partial     )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( op == ErfOp || op == ErfcOp );
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 5 );
   CPPAD_ASSERT_UNKNOWN( d < cap_order );
   CPPAD_ASSERT_UNKNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >= i_z + 2
   );

   // array used to pass parameter values for sub-operations
   addr_t addr[2];

   // If pz is zero, make sure this operation has no effect
   // (zero times infinity or nan would be non-zero).
   Base* pz  = partial + i_z * nc_partial;
   bool skip(true);
   for(size_t i_d = 0; i_d <= d; i_d++)
      skip &= IdenticalZero(pz[i_d]);
   if( skip )
      return;

   // convert from final result to first result
   i_z -= 4; // 4 = NumRes(ErfOp) - 1;

   // Taylor coefficients and partials corresponding to x
   const Base* x  = taylor  + size_t(arg[0]) * cap_order;
   Base* px       = partial + size_t(arg[0]) * nc_partial;

   // Taylor coefficients and partials corresponding to z_3
   const Base* z_3  = taylor  + (i_z+3) * cap_order;
   Base* pz_3       = partial + (i_z+3) * nc_partial;

   // Taylor coefficients and partials corresponding to z_4
   Base* pz_4 = partial + (i_z+4) * nc_partial;

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
   addr[0] = arg[1];            // 2 / sqrt(pi)
   addr[1] = addr_t( i_z + 2 ); // z_2
   reverse_mulpv_op(
      d, i_z+3, addr, parameter, cap_order, taylor, nc_partial, partial
   );

   // z_2 = exp( - x * x )
   reverse_exp_op(
      d, i_z+2, i_z+1, cap_order, taylor, nc_partial, partial
   );

   // z_1 = - x * x
   reverse_neg_op(
      d, i_z+1, i_z+0, cap_order, taylor, nc_partial, partial
   );

   // z_0 = x * x
   addr[0] = arg[0]; // x
   addr[1] = arg[0]; // x
   reverse_mulvv_op(
      d, i_z+0, addr, parameter, cap_order, taylor, nc_partial, partial
   );

}


}}} // END namespace
# endif // CPPAD_ERF_OP_INCLUDED
