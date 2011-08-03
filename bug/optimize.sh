#! /bin/bash -e
#
cat << EOF 
This shell script will create the program named ./optimize.
It will then run ./optimize which generates the following error message:
	Error detected by false result for
		check[i] == y[i]
	at line 1719 in the file 
		../cppad/local/optimize.hpp
------------------------------------------------------------------------
EOF
#
echo "create optimize.cpp"
cat << EOF > optimize.cpp
# include <cppad/cppad.hpp>

int main(void)
{	bool ok = true;
	using CppAD::AD;

	// independent variable vector
	size_t n = 5;
	CPPAD_TEST_VECTOR< AD<double> > ax(n);
	size_t j;
	for(j = 0; j < n; j++)
		ax[j] = 1. / 3.;
	CppAD::Independent(ax);

	// dependent variable vector
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > ay(m);
	ay[0]       = 0.;
	for(j = 0; j < n; j++)
	{	if( j % 2 == 0 )
			ay[0] += ax[j];
		else	ay[0] -= ax[j];
	}
	CppAD::ADFun<double> f(ax, ay);

	f.optimize();

	if( ! ok )
		return 1;
	return 0;
}
EOF
echo "g++ -g -I.. -Wall optimize.cpp -o optimize"
g++ -g -I.. -Wall optimize.cpp -o optimize
#
echo "./optimize"
./optimize
#
exit 0
