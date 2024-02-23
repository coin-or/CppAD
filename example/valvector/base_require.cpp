# include <cppad/example/valvector/class.hpp>
# include <cppad/cppad.hpp>
//
// base_require
bool base_require(void)
{  // ok
   bool ok = true;
   //
   // x
   std::vector<valvector> x(2);
   //
   // valvector(double)
   x[0] = valvector( 2.0 );
   //
   // valvector( list_of_double )
   x[1] = valvector( {-3.0, 4.0, 5.0} );
   //
   // EqualOpSeq
   ok &= ! CppAD::EqualOpSeq( x[0], x[1] );
   //
   // numeric_limits
   valvector nan = CppAD::numeric_limits<valvector>::quiet_NaN();
   ok &= CppAD::isnan( nan );
   //
   // result, check
   valvector result, check;
   //
   // binary operator
   result = x[0] * x[1];
   check  = valvector( {-6.0, 8.0, 10.0} );
   ok &= result == check;
   //
   // std::math
   result = CppAD::fabs( x[1] );
   check  = valvector( {3.0, 4.0, 5.0} );
   ok &= result == check;
   //
   // compound assignment
   result  = x[1];
   result += x[0];
   check   = valvector( {-1.0, 6.0, 7.0} );
   ok &= result == check;
   //
   // conditional expression
   valvector left      = x[0];
   valvector right     = x[1];
   valvector if_true   = x[0] + x[1];
   valvector if_false  = x[0] - x[1];
   result = CppAD::CondExpGe(left, right, if_true, if_false);
   check  = valvector( {-1.0, -2.0, -3.0} );
   ok &= result == check;
   //
   // unary operator
   result = - x[1];
   check  = valvector( {3.0, -4.0, -5.0} );
   ok &= result == check;
   //
   // azmul
   left   = valvector( {0.0, 2.0} );
   right  = valvector( { std::numeric_limits<double>::quiet_NaN(), 3.0 } );
   result = CppAD::azmul(left, right);
   check  = valvector( {0.0, 6.0} );
   ok &= result == check;
   //
   // sign
   result = CppAD::sign( x[1] );
   check  = valvector( {-1.0, +1.0, +1.0} );
   ok &= result == check;
   //
   // abs
   result = CppAD::abs( x[1] );
   check  = valvector( {3.0, 4.0, 5.0} );
   ok &= result == check;
   //
   // pow
   left   = valvector( {1.0, 2.0, 3.0} );
   right  = valvector( {3.0, 2.0, 1.0} );
   result = CppAD::pow(left, right);
   check  = valvector( {1.0, 4.0, 3.0} );
   ok &= result == check;
   //
   return ok;
}
