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
   x[1] = valvector( {3.0, 4.0, 5.0} );
   //
   // valvector binary operator
   valvector z(x[0] * x[1]);
   valvector check( {6.0, 8.0, 10.0} );
   ok &= z == check;
   //
   if( ok )
   {  std::cout << "valvector: OK\n";
      return 0;
   }
   std::cout << "valvector: Error\n";
   return 1;
}
