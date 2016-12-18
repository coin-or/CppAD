// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/* File : my_example.c */

# include <ctime>
double My_variable = 3.0;

int fact(int n) {
	if (n <= 1) return 1;
	else return n*fact(n-1);
}

int my_mod(int x, int y) {
	return (x%y);
}

char *get_time()
{
	time_t ltime;
	time(&ltime);
	return ctime(&ltime);
}
