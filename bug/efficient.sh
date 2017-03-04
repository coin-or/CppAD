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
before_version='6ce3cba095b850be40918f34cb69661624177ebd'
after_version='14a35c1f3d44df5090fd35d1f8fe3ce0d4e18fc9'
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
        // temporary variables
        std::vector<AD<double> > v(17);

        v[0] = ind[8] * ind[10];
        v[1] = ind[9] * ind[11];
        v[2] = 0.1768325928384763 * ind[0];
        v[3] = 0.0180152833 * 1000 * (-13.851 + 0.64038 * ind[2] + -0.00191 * ind[2] * ind[2] + 1.8211e-06 * ind[2] * ind[2] * ind[2]);
        v[4] = v[2] * v[3];
        v[5] = ((v[4] - 1.0e-15 * ind[1] * v[2]) / 0.0180152833) / v[2];
        v[6] = (v[5] * v[2]) / (ind[1] * v[2] + v[5] * v[2]);
        v[7] = 1 - v[6];
        v[8] = (0.0180152833 * 1000 * (-13.851 + 0.64038 * ind[2] + -0.00191 * ind[2] * ind[2] + 1.8211e-06 * ind[2] * ind[2] * ind[2]) * ind[5]) / (1.0e-15 * v[7] + 0.0180152833 * v[6]);
        v[9] = exp(log(ind[6]) - (8.31447215 * ind[7]) / (8.31447215 * ind[2]));
        v[7] = v[0] + v[1] - v[7] * v[8] + -1 * v[9] * v[2];
        v[10] = 0.0180152833 * 1000 * (-13.851 + 0.64038 * ind[12] + -0.00191 * ind[12] * ind[12] + 1.8211e-06 * ind[12] * ind[12] * ind[12]) * ind[10];
        v[11] = 0.0180152833 * 1000 * (-13.851 + 0.64038 * ind[13] + -0.00191 * ind[13] * ind[13] + 1.8211e-06 * ind[13] * ind[13] * ind[13]) * ind[11];
        v[12] = ind[12] * ind[12];
        v[13] = ind[2] * ind[2];
        v[14] = ind[13] * ind[13];
        v[15] = ind[2] * ind[2];
        v[16] = ind[22] * (0.1768325928384763 + 1.4906857141283567 * ind[0]) * (ind[2] - ind[3]);
        dxdt[2] = (1.0e-15 * v[0] * ind[15] * (ind[12] - ind[2]) + v[10] * (15340.863387921299 * (ind[12] - ind[2]) + -58.009079435348092 * (ind[12] * ind[12] - ind[2] * ind[2]) + 0.1503353174209219 * (ind[12] * ind[12] * ind[12] - ind[2] * ind[2] * ind[2]) + -0.00019588923145049848 * (ind[12] * ind[12] * ind[12] * ind[12] - ind[2] * ind[2] * ind[2] * ind[2]) + 1.0402389841962685e-07 * (v[12] * v[12] * ind[12] - v[13] * v[13] * ind[2])) + 1.0e-15 * v[1] * ind[15] * (ind[13] - ind[2]) + v[11] * (15340.863387921299 * (ind[13] - ind[2]) + -58.009079435348092 * (ind[13] * ind[13] - ind[2] * ind[2]) + 0.1503353174209219 * (ind[13] * ind[13] * ind[13] - ind[2] * ind[2] * ind[2]) + -0.00019588923145049848 * (ind[13] * ind[13] * ind[13] * ind[13] - ind[2] * ind[2] * ind[2] * ind[2]) + 1.0402389841962685e-07 * (v[14] * v[14] * ind[13] - v[15] * v[15] * ind[2])) - ind[16] * v[9] * v[2] + 0 - v[16]) / (v[4] * (1.0e-15 * ind[1] * ind[15] + 0.0180152833 * v[5] * (15340.863387921299 + -116.01815887069618 * ind[2] + 0.45100595226276569 * ind[2] * ind[2] + -0.00078355692580199391 * ind[2] * ind[2] * ind[2] + 5.2011949209813426e-07 * ind[2] * ind[2] * ind[2] * ind[2])) / (1.0e-15 * ind[1] + 0.0180152833 * v[5]) + ind[27]);
        v[11] = (1.0e-15 * v[7] + 0.0180152833 * (v[10] / 0.0180152833 + v[11] / 0.0180152833 - v[6] * v[8]) - v[2] * 0.0180152833 * 1000 * (0.64038 + -0.00382 * ind[2] + 5.4633e-06 * ind[2] * ind[2]) * dxdt[2]) / v[3];
        dxdt[0] = v[11] / 0.1768325928384763;
        dxdt[1] = (v[7] - ind[1] * v[11]) / v[2];
        v[11] = ind[17] * ind[17];
        v[7] = ind[3] * ind[3];
        dxdt[3] = (0.0180152833 * 1000 * (-13.851 + 0.64038 * ind[17] + -0.00191 * ind[17] * ind[17] + 1.8211e-06 * ind[17] * ind[17] * ind[17]) * ind[4] * (15340.863387921299 * (ind[17] - ind[3]) + -58.009079435348092 * (ind[17] * ind[17] - ind[3] * ind[3]) + 0.1503353174209219 * (ind[17] * ind[17] * ind[17] - ind[3] * ind[3] * ind[3]) + -0.00019588923145049848 * (ind[17] * ind[17] * ind[17] * ind[17] - ind[3] * ind[3] * ind[3] * ind[3]) + 1.0402389841962685e-07 * (v[11] * v[11] * ind[17] - v[7] * v[7] * ind[3])) + v[16]) / (0.0180152833 * (0.0180152833 * 1000 * (-13.851 + 0.64038 * ind[3] + -0.00191 * ind[3] * ind[3] + 1.8211e-06 * ind[3] * ind[3] * ind[3])) / 0.0180152833 * ind[26] * (15340.863387921299 + -116.01815887069618 * ind[3] + 0.45100595226276569 * ind[3] * ind[3] + -0.00078355692580199391 * ind[3] * ind[3] * ind[3] + 5.2011949209813426e-07 * ind[3] * ind[3] * ind[3] * ind[3]) + ind[19]);
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
    size_t repeat = 6;

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

    std::vector<AD<double> > dxikdt(ns_);
    std::vector<AD<double> > xik(na_);

    // parameters
    for (size_t j = 0; j < npar_; j++)
        xik[ns_ + nm_ + j] = x[j];

    size_t s = npar_;
    size_t nvarsk = ns_;
    size_t nMstart = npar_ + nvarsk * K_ * repeat + nvarsk;
    size_t eq = 0;

    for (size_t i = 0; i < repeat; i++) {
        size_t s0 = s;

        // controls
        for (size_t j = 0; j < nm_; j++) {
            xik[ns_ + j] = x[nMstart + nm_ * i + j];
        }

        // K = 1
        for (size_t j = 0; j < ns_; j++) {
            xik[j] = x[s + j]; // states
        }
        s += nvarsk;
        // xik[ns + nm + npar] = x[s + ns];// time

        atomModel(xik, dxikdt); // ODE
        for (size_t j = 0; j < ns_; j++) {
            dep[eq + j] = dxikdt[j]
                          + 0.13797958971132715 * x[s0 + j]
                          + -0.10749149571305303 * x[s0 + nvarsk + j]
                          + -0.038928002823013501 * x[s0 + 2 * nvarsk + j]
                          + 0.008439908824739363 * x[s0 + 3 * nvarsk + j];
        }
        eq += ns_;

        // K = 2
        for (size_t j = 0; j < ns_; j++) {
            xik[j] = x[s + j]; // states
        }
        s += nvarsk;
        // xik[ns + nm + npar] = x[s + ns];// time

        atomModel(xik, dxikdt); // ODE
        for (size_t j = 0; j < ns_; j++) {
            dep[eq + j] = dxikdt[j]
                          + -0.057979589711327127 * x[s0 + j]
                          + 0.11892800282301351 * x[s0 + nvarsk + j]
                          + -0.025841837620280327 * x[s0 + 2 * nvarsk + j]
                          + -0.035106575491406049 * x[s0 + 3 * nvarsk + j];
        }
        eq += ns_;

        // K = 3
        for (size_t j = 0; j < ns_; j++) {
            xik[j] = x[s + j]; // states
        }
        s += nvarsk;
        // xik[ns + nm + npar] = x[s + ns];// time

        atomModel(xik, dxikdt); // ODE
        for (size_t j = 0; j < ns_; j++) {
            dep[eq + j] = dxikdt[j]
                          + 0.099999999999999978 * x[s0 + j]
                          + -0.18439908824739357 * x[s0 + nvarsk + j]
                          + 0.25106575491406025 * x[s0 + 2 * nvarsk + j]
                          + -0.16666666666666669 * x[s0 + 3 * nvarsk + j];
        }
        eq += ns_;

    }

    return dep;
}
EOF
# -----------------------------------------------------------------------------
cp bug/efficient.sh ~/trash
git checkout bug/efficient.sh
#
before_date=`git show -q $before_version | grep Date | \
	sed -e 's|Date: *[^ ]* *||' -e 's|[^ ]*$||'`
after_date=`git show -q $after_version | grep Date | \
	sed -e 's|Date: *[^ ]* *||' -e 's|[^ ]*$||'`
#
echo_eval git checkout -q $before_version
echo "bin/run_cmake.sh > efficient.log"
bin/run_cmake.sh > bug/efficient.log
cd bug/build
echo_eval g++ -I${cppad_path} --std=c++11 -g test_jac_nnz.cpp -o test_jac_nnz
before_nnz=`./test_jac_nnz`
echo "before_nnz = $before_nnz, before_date = $before_date"
cd ../..
#
echo_eval git checkout -q $after_version
echo "bin/run_cmake.sh >> efficient.log"
bin/run_cmake.sh >> bug/efficient.log
cd bug/build
echo_eval g++ -I${cppad_path} --std=c++11 -g test_jac_nnz.cpp -o test_jac_nnz
after_nnz=`./test_jac_nnz`
echo "after_nnz = $after_nnz, after_date = $after_date"
cd ../..
echo_eval git checkout -q master
cp ~/trash/efficient.sh bug/.
#
for file in example/test_one.sh test_more/test_one.sh
do
	if [ -e $file ]
	then
		rm $file
	fi
done
