#! /bin/bash -e
# $Id:$
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
CppAD::vector<CppAD::AD<double> > evaluateModel(
	const CppAD::vector<CppAD::AD<double> >& x,
	CppAD::atomic_base<double>& atomModel
);

const size_t ns_ = 4;

namespace {
    void algo(
		const CppAD::vector <AD<double>>& ind ,
		CppAD::vector <AD<double>>&      dxdt )
	{
        dxdt[2] = 1.0;
        dxdt[0] = 1.0;
        dxdt[1] = ind[2];
        dxdt[3] = ind[2];
    };
}

int main(int argc, char *argv[]) {

    CppAD::vector<AD<double>> ay(ns_), ax(ns_);
    for(size_t i = 0; i < ns_; ++i)
        ax[i] = 1.0;

    checkpoint<double> atomic_fun(
		"cstr",
		algo,
		ax,
		ay,
		atomic_base<double>::set_sparsity_enum
	);

    CppAD::vector <AD<double>> u(ns_);
    for (size_t j = 0; j < ns_; j++)
        u[j] = 1.0;

    CppAD::Independent(u);

    CppAD::vector<AD<double>> v = evaluateModel(u, atomic_fun);

    ADFun<double> fun;
    fun.Dependent(v);

    /**
     * determine Jacobian sparsity
     */
    size_t m = fun.Range();

    CppAD::vector<std::set<size_t> > s_s(m);
    for (size_t i = 0; i < m; i++)
        s_s[i].insert(i);

    auto sparsity = fun.RevSparseJac(m, s_s);

    size_t nnz = 0;
    for (size_t i = 0; i < sparsity.size(); i++) {
		std::set<size_t> s_i = sparsity[i];
        nnz += s_i.size();
    }
	std::cout << nnz << "\n";
    return 0;
}

CppAD::vector<CppAD::AD<double> > evaluateModel(
	const CppAD::vector<CppAD::AD<double> >& x,
	CppAD::atomic_base<double>&            atom_fun )
{
    // dependent variable vector
    CppAD::vector<AD<double> > dep(ns_), dxdt(ns_);
	atom_fun(x, dxdt); // ODE
	for (size_t j = 0; j < ns_; j++) {
		dep[j] = dxdt[j] +  x[j];
	}

	return dep;
}
EOF
# -----------------------------------------------------------------------------
git checkout cppad/local/sparse_list.hpp
sed -e '505s|^|check_data_not_used();\n|' -i cppad/local/sparse_list.hpp
cd bug/build
echo_eval g++ -I${cppad_path} --std=c++11 -g test_jac_nnz.cpp -o test_jac_nnz
echo_eval ./test_jac_nnz
