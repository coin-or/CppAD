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
#include <assert.h>
#include <vector>

using namespace CppAD;

std::vector<CppAD::AD<double> > evaluateModel(const std::vector<CppAD::AD<double> >& x,
                                              size_t repeat,
                                              CppAD::atomic_base<double>& atomModel);

std::vector<double> getTypicalValues(std::vector<double> xa,
                                     size_t repeat);

const size_t K_ = 3;
const size_t ns_ = 4;
const size_t nm_ = 2;
const size_t npar_ = 22;
const size_t na_ = ns_ + nm_ + npar_;

int main(int argc, char *argv[]) {
    /**
     * create atomic function
     */
    auto atomicFunction = [](const std::vector <AD<double>>& ind,
                             std::vector <AD<double>>& dxdt) {
        dxdt[2] = 1.0;
        dxdt[0] = 1.0;
        dxdt[1] = ind[2];
        dxdt[3] = ind[2];
    };

    std::vector<double> xx(na_);
    xx[0] = 0.3; // h
    xx[1] = 7.82e3; // Ca
    xx[2] = 304.65; // Tr
    xx[3] = 301.15; // Tj
    xx[4] = 2.3333e-04; // u1
    xx[5] = 6.6667e-05; // u2
    xx[6] = 6.2e14; //
    xx[7] = 10080; //
    xx[8] = 2e3; //
    xx[9] = 10e3; //
    xx[10] = 1e-11; //
    xx[11] = 6.6667e-05; //
    xx[12] = 294.15; //
    xx[13] = 294.15; //
    xx[14] = 1000; //
    xx[15] = 4184; //Cp
    xx[16] = -33488; //deltaH
    xx[17] = 299.15; // Tj0
    xx[18] = 302.65; //   Tj2
    xx[19] = 7e5; // cwallj
    xx[20] = 1203; // csteam
    xx[21] = 3.22; //dsteam
    xx[22] = 950.0; //Ug
    xx[23] = 0.48649427192323; //vc6in
    xx[24] = 1000; //rhoj
    xx[25] = 4184; //Cpj
    xx[26] = 0.014; //Vj
    xx[27] = 1e-7; //cwallr

    std::vector<AD<double>> ay(ns_), ax(na_);
    for(size_t i = 0; i < na_; ++i)
        ax[i] = xx[i];

    checkpoint<double> atomic("cstr",
                              atomicFunction,
                              ax,
                              ay,
                              atomic_base<double>::set_sparsity_enum);

    /**
     * create tape
     */
    size_t repeat = 1;

    std::vector<double> xTypical = getTypicalValues(xx, repeat);
    size_t n = xTypical.size();
    std::vector <AD<double>> u(n);
    for (size_t j = 0; j < n; j++)
        u[j] = xTypical[j];

    CppAD::Independent(u);

    std::vector<AD<double>> v = evaluateModel(u, repeat, atomic);

    ADFun<double> fun;
    fun.Dependent(v);

    /**
     * determine Jacobian sparsity
     */
    size_t m = fun.Range();

    std::vector<std::set<size_t> > s_s(m);
    for (size_t i = 0; i < m; i++)
        s_s[i].insert(i);

    auto sparsity = fun.RevSparseJac(m, s_s);

    size_t nnz = 0;
    for (const auto& s: sparsity) {
        nnz += s.size();
    }
	std::cout << nnz << "\n";
    return 0;
}

std::vector<double> getTypicalValues(std::vector<double> xa,
                                     size_t repeat) {
    /**
     * collocation model values
     */
    size_t nvarsk = ns_;
    size_t nMstart = npar_ + nvarsk * K_ * repeat + nvarsk;

    std::vector<double> x(nMstart + repeat * nm_, 1.0);
    // parameters
    for (size_t j = 0; j < npar_; j++)
        x[j] = xa[ns_ + nm_ + j];

    size_t s = npar_;

    // i = 0 K = 0
    // states
    for (size_t j = 0; j < ns_; j++) {
        x[s++] = xa[j];
    }

    for (size_t i = 0; i < repeat; i++) {
        // controls
        for (size_t j = 0; j < nm_; j++) {
            x[nMstart + nm_ * i + j] = xa[ns_ + j];
        }

        // K = 1
        // states
        for (size_t j = 0; j < ns_; j++) {
            x[s] = 1.0 + 0.01 * i;
            x[s++] = xa[j];
        }

        // K = 2
        // states
        for (size_t j = 0; j < ns_; j++) {
            x[s++] = xa[j];
        }

        // K = 3
        // states
        for (size_t j = 0; j < ns_; j++) {
            x[s++] = xa[j];
        }
    }

    return x;
}

std::vector<CppAD::AD<double> > evaluateModel(const std::vector<CppAD::AD<double> >& x,
                                              size_t repeat,
                                              CppAD::atomic_base<double>& atomModel) {
    size_t m2 = repeat * K_ * ns_;

    // dependent variable vector
    std::vector<AD<double> > dep(m2);
	for(size_t i = 0; i < m2; i++)
		dep[i] = 0.0;

    std::vector<AD<double> > dxikdt(ns_);
    std::vector<AD<double> > xik(na_);
	for(size_t j = 0; j < na_; j++)
		xik[j] = 1.0;

    // parameters
    for (size_t j = 0; j < npar_; j++)
        xik[ns_ + nm_ + j] = x[j];

    size_t s = npar_;
    size_t nvarsk = ns_;
    size_t nMstart = npar_ + nvarsk * K_ * repeat + nvarsk;
    size_t eq = 0;

	size_t i = 0;
    {
        size_t s0 = s;

        s += 2 * nvarsk;
        eq += 2 * ns_;

        // K = 3
        for (size_t j = 0; j < ns_; j++) {
            xik[j] = x[s + j]; // states
        }

        atomModel(xik, dxikdt); // ODE
        for (size_t j = 0; j < ns_; j++) {
            dep[j] = dxikdt[j] +  x[s0 + 2 * nvarsk + j];
        }

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
#
