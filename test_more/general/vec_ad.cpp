// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
Old examples now only used for validation testing
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>
# include <limits>
# include <cppad/speed/uniform_01.hpp>


namespace { // Begin empty namespace

void myhandler(
   bool known       ,
   int  line        ,
   const char *file ,
   const char *exp  ,
   const char *msg  )
{  // error handler must not return, so throw an exception
   throw msg;
}

bool test_comp_assign(void)
{  bool ok = true;

   // replace the default CppAD error handler
   CppAD::ErrorHandler info(myhandler);


   // create a VecAD vector
   CppAD::VecAD<double> v(1);

   // assign its element a value
   v[0] = 1.0;

# ifndef NDEBUG
   // use try / catch because error haandler throws an exception
   try {
      // set ok to false unless catch block is executed
      ok = false;

      // attempt to use a compound assignment operator
      // with a reference to a VecAD object
      CppAD::AD<double> x = 0.0;
      v[x] += 1.0;
   }
   catch (const char* msg)
   {  std::string check =
         "Can't use VecAD<Base>::reference on left side of +=";
      ok = msg == check;
   }
# endif

   return ok;
}

bool VecADTestOne(void)
{  bool ok = true;
   using namespace CppAD;
   using CppAD::sin;
   using CppAD::cos;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();


   size_t      n = 3;
   AD<double>  N(n);

   AD<double>  a;
   size_t      i;

   // create the array
   CppAD::VecAD<double> V(n);

   // check assignment from double (while not taping)
   for(i = 0; i < n; i++)
      V[i] = double(n - i);

   // check assignment from an AD<double> (while not taping)
   for(i = 0; i < n; i++)
      V[i] = 2. * V[i];

   // check array values (while not taping)
   for(i = 0; i < n; i++)
      ok &= ( V[i] == 2. * double(n - i) );

   // independent variable
   CPPAD_TESTVECTOR(AD<double>) X(1);
   X[0] = double(n - 1);
   Independent(X);

   // check assignment from double during taping
   a = -1.;
   for(i = 0; i < n; i++)
   {  a += 1.;
      V[a] = double(n - i);
   }

   // check assignment from AD<double> during taping
   a = -1.;
   for(i = 0; i < n; i++)
   {  a += 1.;
      V[a] = sin( X[0] ) * V[a];
   }

   // dependent variable
   CPPAD_TESTVECTOR(AD<double>) Z(1);
   Z[0] = V[ X[0] ];

   // create f: X -> Z
   ADFun<double> f(X, Z);
   CPPAD_TESTVECTOR(double)  x( f.Domain() );
   CPPAD_TESTVECTOR(double) dx( f.Domain() );
   CPPAD_TESTVECTOR(double)  z( f.Range() );
   CPPAD_TESTVECTOR(double) dz( f.Range() );

   double vx;
   for(i = 0; i < n; i++)
   {  // check that the indexing operation was taped
      x[0] = double(i);
      z    = f.Forward(0, x);
      vx   = double(n - i);
      ok  &= NearEqual(z[0], sin(x[0]) * vx, eps99, eps99);

      // note that derivative of v[x] w.r.t. x is zero
      dx[0] = 1.;
      dz    = f.Forward(1, dx);
      ok   &= NearEqual(dz[0], cos(x[0]) * vx, eps99, eps99);

      // reverse mode calculation of same value
      dz[0] = 1.;
      dx    = f.Reverse(1, dz);
      ok   &= NearEqual(dx[0], cos(x[0]) * vx, eps99, eps99);
   }


   return ok;
}

// create the discrete function AD<double> Floor(const AD<double> &X)
double Floor(const double &x)
{  return std::floor(x); }
CPPAD_DISCRETE_FUNCTION(double, Floor)

bool VecADTestTwo(void)
{  bool ok = true;
   using namespace CppAD;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   double pi    = 4. * CppAD::atan(1.);
   size_t nx    = 10;                             // number of x grid point
   double xLow  = 0;                              // minimum value for x
   double xUp   = 2 * pi;                         // maximum value for x
   double xStep = (xUp - xLow) / double(nx - 1);  // step size in x
   double xCur;                                   // current value for x

   // fill in the data vector on a uniform grid
   VecAD<double> Data(nx);
   size_t i;
   for(i = 0; i < nx; i++)
   {  xCur = xLow + double(i) * xStep;
      // use size_t indexing of Data while not taping
      Data[i] = CppAD::sin(xCur);
   }

   // declare independent variable
   CPPAD_TESTVECTOR(AD<double>) X(1);
   X[0] = 2.;
   Independent(X);

   // compute index corresponding to current value of X[0]
   AD<double> I = X[0] / xStep;
   AD<double> Ifloor = Floor(I);

   // make sure Ifloor >= 0  (during taping)
   AD<double> Zero(0);
   Ifloor = CondExpLt(Ifloor, Zero, Zero, Ifloor);

   // make sure Ifloor <= nx - 2 (during taping)
   AD<double> Nxminus2(nx - 2);
   Ifloor = CondExpGt(Ifloor, Nxminus2, Nxminus2, Ifloor);

   // Iceil is Ifloor + 1
   AD<double> Iceil  = Ifloor + 1.;

   // linear interpolate Data
   CPPAD_TESTVECTOR(AD<double>) Y(1);
   Y[0] = Data[Ifloor] + (I - Ifloor) * (Data[Iceil] - Data[Ifloor]);

   // create f: X -> Y that linearly interpolates the data vector
   ADFun<double> f(X, Y);

   // evaluate the linear interpolant at the mid point for first interval
   CPPAD_TESTVECTOR(double)  x(1);
   CPPAD_TESTVECTOR(double)  y(1);
   x[0] = xStep / 2.;
   y    = f.Forward(0, x);
   ok  &= NearEqual(y[0], (Data[0] + Data[1])/2., eps99, eps99);

   // evalute the derivative with respect to x
   CPPAD_TESTVECTOR(double) dx(1);
   CPPAD_TESTVECTOR(double) dy(1);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], (Data[1] - Data[0]) / xStep, eps99, eps99);

   return ok;
}

bool SecondOrderReverse(void)
{  // Bradley M. Bell 2009-07-06
   // Reverse mode for LdpOp was only modifying the highest order partial
   // This test demonstrated the bug
   bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * std::numeric_limits<double>::epsilon();

   size_t n = 1;
   CPPAD_TESTVECTOR(AD<double>) X(n);
   X[0] = 2.;
   CppAD::Independent(X);

   size_t m = 2;
   CPPAD_TESTVECTOR(AD<double>) Y(m);

   // The LdpOp instruction corresponds to operations with VecAD vectors.
   CppAD::VecAD<double> Z(2);
   AD<double> zero = 0;
   Z[zero] = X[0] + 1;

   // The LdvOp instruction corresponds to the index being a variable.
   AD<double> one = X[0] - 1; // one in a variable here
   Z[one]  = X[0] + 1.;


   // Compute a function where the second order partial for y
   // depends on the first order partials for z
   // This will use the LdpOp instruction because the index
   // access to z is the parameter zero.
   Y[0] = Z[zero] * Z[zero];
   Y[1] = Z[one]  * Z[one];

   CppAD::ADFun<double> f(X, Y);

   // first order forward
   CPPAD_TESTVECTOR(double) dx(n);
   size_t p = 1;
   dx[0]    = 1.;
   f.Forward(p, dx);

   // Test LdpOp
   // second order reverse (test exp_if_true case)
   CPPAD_TESTVECTOR(double) w(m), dw(2 * n);
   w[0] = 1.;
   w[1] = 0.;
   p    = 2;
   dw = f.Reverse(p, w);

   // check first derivative in dw
   double check = 2. * (Value( X[0] ) + 1.);
   ok &= NearEqual(dw[0], check, eps, eps);

   // check second derivative in dw
   check = 2.;
   ok &= NearEqual(dw[1], check, eps, eps);

   // Test LdvOp
   // second order reverse (test exp_if_true case)
   w[0] = 0.;
   w[1] = 1.;
   p    = 2;
   dw = f.Reverse(p, w);

   // check first derivative in dw
   check = 2. * (Value( X[0] ) + 1.);
   ok &= NearEqual(dw[0], check, eps, eps);

   // check second derivative in dw
   check = 2.;
   ok &= NearEqual(dw[1], check, eps, eps);

   return ok;
}

/*
get_test_function
Function that uses all the VecAD operators; i.e.
StppOp, StpvOp, StvpOp, StvvOp, LdOpOp, LdvOp

The funcntion v(x) is defined by
v_0(x) = 6.0
v_1(x) = x_2
v_2(x) = if int(x_0) == 2 then      x_3
         else if int(x_1) == 2 then 5.0
         else                       2.0
v_3(x) = if int(x_0) == 3 then      x_3
         else if int(x_1) == 3 then 5.0
         else                       3.0

The function y(x) is defined by
y_i(x) = if i < 4 then v_i(x) * v_i(x)
         else v_k(x) * v_k(x) where k = int(x_4)
*/
CppAD::ADFun<double> get_test_function(void)
{  //
   // AD, VecAD
   using CppAD::AD;
   //
   // ax
   CPPAD_TESTVECTOR( AD<double> ) ax(5);
   for(size_t i = 0; i < 5; ++i)
      ax[i] = 2.0;
   CppAD::Independent(ax);
   //
   // av (parameter)
   CppAD::VecAD<double> av(4);
   for(size_t i = 0; i < 4; ++i)
      av[i] = double(i);
   //
   // av (variable)
   AD<double> azero(0);
   AD<double> aone(1);
   av[ ax[1] ] = 5.0;    // StvpOp
   av[ ax[0] ] = ax[3];  // StvvOp
   av[azero]   = 6.0;    // StppOp
   av[aone]    = ax[2];  // StpvOp
   //
   // ay
   CPPAD_TESTVECTOR( AD<double> ) ay(5);
   for(size_t i = 0; i < 4; ++i)
   {  AD<double> ai(i);
      ay[i] = av[ai] * av[ai]; // LdpOp
   }
   ay[4] = av[ ax[4] ] * av[ ax[4] ];   // LdvOp
   //
   // test_function
   return CppAD::ADFun<double>(ax, ay);
}
//
// get_test_function_x
CPPAD_TESTVECTOR(double) get_test_function_x(void)
{  //
   // seed
   using std::chrono::seconds;
   seconds sec = std::chrono::duration_cast<seconds>(
      std::chrono::system_clock::now().time_since_epoch()
   );
   unsigned int seed = static_cast<unsigned int>( sec.count() );
   //
   // uniform_01
   CppAD::uniform_01(seed);
   //
   // x
   CPPAD_TESTVECTOR(double) x(5);
   CppAD::uniform_01(x.size(), x);
   for(size_t i = 0; i < 5; ++i)
      x[i] = 3.999999 * x[i];
   //
   return x;
}
//
// forward_zero_test_function
bool forward_zero_test_function(
   CppAD::ADFun<double>&     fun ,
   CPPAD_TESTVECTOR(double)& x   )
{  //
   // ok
   bool ok = true;
   //
   // eps99
   double eps99 = std::numeric_limits<double>::epsilon();
   //
   // y
   CPPAD_TESTVECTOR(double) y(5);
   y = fun.Forward(0, x);
   //
   // v
   CPPAD_TESTVECTOR(double) v(4);
   v[0] = 6.0;
   v[1] = x[2];
   // v[2]
   if( int(x[0]) == 2 )
      v[2] = x[3];
   else if( int(x[1]) == 2 )
      v[2] = 5.0;
   else
      v[2] = 2.0;
   // v[3]
   if( int(x[0]) == 3 )
      v[3] = x[3];
   else if( int(x[1]) == 3 )
      v[3] = 5.0;
   else
      v[3] = 3.0;
   //
   // ok
   for(size_t i = 0; i < 4; ++i)
      ok &= CppAD::NearEqual(y[i], v[i] * v[i], eps99, eps99);
   size_t k = size_t(x[4]);
   ok &= CppAD::NearEqual(y[4], v[k] * v[k], eps99, eps99);
   //
   return ok;
}

// forward_one_test_function
bool forward_one_test_function(
   CppAD::ADFun<double>&     fun ,
   CPPAD_TESTVECTOR(double)& x   )
{  //
   // ok
   bool ok = true;
   //
   // eps99
   double eps99 = std::numeric_limits<double>::epsilon();
   //
   // fun
   fun.Forward(0, x);
   //
   // dx, dy
   CPPAD_TESTVECTOR(double) dx(5), dy(5);
   for(size_t j = 0; j < 5; ++j)
   {  // dx
      for(size_t k = 0; k < 5; ++k)
         dx[k] = 0.0;
      dx[j] = 1.0;
      //
      // dy
      dy = fun.Forward(1, dx);
      //
      // ok
      // for dy_i / dx_j for i = 0, ..., 3
      for(size_t i = 0; i < 4; ++i)
      {  if( j == 0 || j == 1 )
            ok &= dy[i] == 0.0;
         else if( j == 2 )
         {  if( i == 1 )
               ok &= CppAD::NearEqual(dy[i], 2.0 * x[2], eps99, eps99);
            else
               ok &= dy[i] == 0.0;
         }
         else if( j == 3 )
         {  if( size_t(x[0]) == i )
            {  if( i == 2 || i == 3 )
                  ok &= CppAD::NearEqual(dy[i], 2.0 * x[3], eps99, eps99);
               else
                  ok &= dy[i] == 0.0;
            }
         }
         else
         {   assert( j == 4 );
             ok &= dy[i] == 0.0;
         }
      }
      //
      // ok
      // for dy_4 / dx_j
      if( j == 0 || j == 1 || j == 4 )
            ok &= dy[4] == 0.0;
      else
      {  size_t k = size_t( x[4] );
         if( k == 1 && j == 2 )
            ok &= CppAD::NearEqual(dy[4], 2.0 * x[2], eps99, eps99);
         else if( (k == 2 || k == 3) && j == 3 && size_t(x[0]) == k )
            ok &= CppAD::NearEqual(dy[4], 2.0 * x[3], eps99, eps99);
         else
            ok &= dy[4] == 0.0;
      }
   }
   //
   return ok;
}

bool jac_sparsity_test_function(CppAD::ADFun<double>& fun)
{  //
   // ok
   bool ok = true;
   //
   // size_vector
   typedef CppAD::vector<size_t> size_vector;
   //
   // sparse_rc
   typedef CppAD::sparse_rc<size_vector> sparse_rc;
   //
   // dependency
   for(bool dependency : {false, true})
   {  //
      // pattern_check
      size_t nr = 5, nc = 5, nnz = 0;
      sparse_rc pattern_check(nr, nc, nnz);
      for(size_t i = 0; i < nr; ++i)
      {  // values in v depend on x_2 and x_3  
         pattern_check.push_back(i, 2);
         pattern_check.push_back(i, 3);
         if( dependency )
         {  // indices in v depend on x_0 and x_1
            pattern_check.push_back(i, 0);
            pattern_check.push_back(i, 1);
            if( i == 4 )
            {  // y_4 depends on x_4 as an index in v
               pattern_check.push_back(i, 4);
            }
         }
      }
      //
      // pattern_eye
      nr = 5, nc = 5, nnz = 5;
      sparse_rc pattern_eye(nr, nc, nnz);
      for(size_t k = 0; k < nr; ++k)
         pattern_eye.set(k, k, k);
      //
      // pattern_jac
      sparse_rc pattern_jac;
      bool transpose     = false;
      bool internal_bool = false;
      fun.for_jac_sparsity(
         pattern_eye, transpose, dependency, internal_bool, pattern_jac
      );
      //
      // ok
      ok &= pattern_jac == pattern_check;
      //
      // pattern_jac
      fun.rev_jac_sparsity(
         pattern_eye, transpose, dependency, internal_bool, pattern_jac
      );
      //
      // ok
      ok &= pattern_jac == pattern_check;
   }
   //
   return ok;
}

} // END empty namespace

bool VecAD(void)
{  bool ok = true;
   ok &= test_comp_assign();
   ok &= VecADTestOne();
   ok &= VecADTestTwo();
   ok &= SecondOrderReverse();
   //
   // fun, x
   CppAD::ADFun<double>     fun = get_test_function();
   CPPAD_TESTVECTOR(double) x   = get_test_function_x();
   //
   ok &= forward_zero_test_function(fun, x);
   ok &= forward_one_test_function(fun, x);
   ok &= jac_sparsity_test_function(fun);
   //
   return ok;
}
