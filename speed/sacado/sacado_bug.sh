#! /bin/bash
# Sacado bug report for trilinos-8.0.5, gcc 3.4.4 
#
# Step 1: 
# change this to the include directory below your version of sacado
sacado_include_directory="$HOME/sacado_base/include"
#
# Step 2
# run the command ./sacado_bug.sh. This creates a segmentation fault.
#
echo "creating sacado_bug.cpp"
cat << EOF > sacado_bug.cpp
# include <Sacado.hpp>
# include <cstddef>

template <typename Scalar, typename Fun>
Scalar Runge45(
	Fun           &F , 
	size_t         M , 
	const Scalar &ti , 
	const Scalar &tf , 
	const Scalar &xi )
{
	static Scalar c5[1] = {
		Scalar(1)
	};
	size_t i, j, m;
	Scalar x5, xf, ftmp;

	xf = xi; 
	for(m = 0; m < M; m++)
	{
		x5 = xf; 
		for(j = 0; j < 6; j++)
		{
			F.Ode(ti, xf, ftmp); 
			x5        += c5[0] * ftmp;
		}
		xf = x5;
	}
	return xf;
}


template <class Float>
class ode_evaluate_fun {
public:
	ode_evaluate_fun() 
	{ }
	void Ode(
		const Float                  &t, 
		Float                        &y, 
		Float                        &g) 
	{
		g  = 1.;
		
	}
};


bool link_ode(void)
{
	// -------------------------------------------------------------
	// setup

	// object for computing determinant
	typedef Sacado::Rad::ADvar<double>   ADScalar; 

	size_t j;
	size_t n = 1;
	ADScalar  X;
	ADScalar  Y;
	ADScalar  last;
	
	X = 1.;

	// set up the case we are integrating
	size_t M  = 2;
	ADScalar  ti = 0.;
	ADScalar  tf = 1.;
	ADScalar  yi;
	ADScalar  yf;

	yi = ADScalar(0);

	// construct ode equation
	ode_evaluate_fun<ADScalar> f;

	// solve differential equation
	yf = Runge45(f, M, ti, tf, yi);

	// make sure function value is last assignment
	last = yf;

	// compute the gradient using reverse mode
	ADScalar::Gradcomp();

	// ---------------------------------------------------------
	return true;
}
// main program that runs all the tests
int main(int argc, char *argv[])
{	bool ok = true;


	link_ode();
	link_ode();


	return static_cast<int>( ! ok );
}
EOF
echo "g++ -g -I$sacado_include_directory sacado_bug.cpp -o sacado_bug"
g++ -g -I$sacado_include_directory sacado_bug.cpp -o sacado_bug
#
echo "./sacado_bug"
./sacado_bug
