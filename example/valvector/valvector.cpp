# include <cppad/example/valvector.hpp>
# include <cppad/cppad.hpp>

int main(void)
{  //
   // vector
   typedef  valvector< std::vector<double> > base;
   typedef  CppAD::AD<base>                  ad_base;
   //
   // x
   std::vector<base> x(2);
   x[0] = base( {2.0} );
   x[1] = base( {3.0, 4.0, 5.0} );
   base z(x[0] * x[1]);
   std::cout << "z = " << z << "\n";
   //
   std::vector<base> ax(x);
   ad_base az(ax[0] * ax[1]);
   std::cout << "az = " << az << "\n";
   //
   return 0;
}
