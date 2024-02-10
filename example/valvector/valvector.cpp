# include <cppad/example/valvector.hpp>
# include <cppad/cppad.hpp>

int main(void)
{  //
   // vector
   typedef  valvector< std::vector<double> > valvec;
   typedef  CppAD::AD<valvec>                ad_valvec;
   //
   // a
   valvec x( {1.0, 2.0, 3.0} );
   valvec y( {1.0, 2.0, 3.0} );
   valvec z(x + y);
   std::cout << "z = " << z << "\n";
   //
   ad_valvec ax(x), ay(y);
   ad_valvec az(ax + ay);
   std::cout << "az = " << az << "\n";
   //
   return 0;
}
