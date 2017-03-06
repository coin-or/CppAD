#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
cd ..
cppad_path=`pwd`
if [ ! -e bug/build ]
then
    mkdir bug/build
fi
#
cat << EOF > bug/build/test_jac_nnz.cpp
#include <cppad/cppad.hpp>

using namespace CppAD;
const size_t ns_ = 4;

namespace {
    void algo(
		const CppAD::vector <AD<double>>& ax ,
		CppAD::vector <AD<double>>&       ay )
	{
        ay[2] = 1.0;
        ay[0] = 1.0;
        ay[1] = ax[2];
        ay[3] = ax[2];
    };
}

int main(int argc, char *argv[]) {
	//
    CppAD::vector<AD<double>> ay(ns_), ax(ns_);
    for(size_t i = 0; i < ns_; ++i)
        ax[i] = 1.0;
	//
    checkpoint<double> atom_fun(
		"cstr",
		algo,
		ax,
		ay,
		atomic_base<double>::set_sparsity_enum
	);
	//
    CppAD::vector <AD<double>> au(ns_);
    for (size_t j = 0; j < ns_; j++)
        au[j] = 1.0;
	//
    CppAD::Independent(au);
	//
    CppAD::vector<AD<double> > av(ns_);
	atom_fun(au, ay); // ODE
	for (size_t j = 0; j < ns_; j++) {
		av[j] = ay[j] +  au[j];
	}
    ADFun<double> fun;
    fun.Dependent(av);
	//
    CppAD::vector<std::set<size_t> > s_s(ns_);
    for (size_t i = 0; i < ns_; i++)
        s_s[i].insert(i);
	//
    auto sparsity = fun.RevSparseJac(ns_, s_s);
	//
    size_t nnz = 0;
    for (size_t i = 0; i < ns_; i++) {
		std::set<size_t> s_i = sparsity[i];
        nnz += s_i.size();
    }
	std::cout << nnz << "\n";
    return 0;
}
EOF
# -----------------------------------------------------------------------------
cd bug/build
echo_eval g++ -I${cppad_path} --std=c++11 -g test_jac_nnz.cpp -o test_jac_nnz
echo_eval ./test_jac_nnz
