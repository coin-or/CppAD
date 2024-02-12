# include <cppad/example/valvector.hpp>
# include <cppad/cppad.hpp>

int main(void)
{  //
   bool ok = true;
   // x
   std::vector<valvector> x(2);
   //
   // valvector(double)
   x[0] = valvector( 2.0 );
   //
   // valvector( list_of_double )
   x[1] = valvector( {-3.0, 4.0, 5.0} );
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
   std::cout << "result = " << result << "\n";
   std::cout << "check  = " << check << "\n";
   //
   if( ok )
   {  std::cout << "valvector: OK\n";
      return 0;
   }
   std::cout << "valvector: Error\n";
   return 1;
}
