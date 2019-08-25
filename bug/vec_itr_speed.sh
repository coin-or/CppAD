#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
name=`echo $0 | sed -e 's|^bug/||' -e 's|\.sh$||'`
if [ "$0" != "bug/$name.sh" ]
then
    echo 'usage: bug/alloc_global.sh'
    exit 1
fi
# -----------------------------------------------------------------------------
if [ -e build/bug ]
then
    rm -r build/bug
fi
mkdir -p build/bug
cd build/bug
# cmake ../..
# -----------------------------------------------------------------------------
cat << EOF
This is not a bug but rather a speed to comparing the speed of CppAD
vector iterators and raw pointer using the std::sort algorithm.
EOF
cat << EOF > $name.cpp
# include <cppad/utility/vector.hpp>
# include <cppad/utility/time_test.hpp>
namespace {
    // declared here so setup does not include allocation
    CppAD::vector<size_t> vec;
    //
    void test_itr(size_t size, size_t repeat)
    {   // size and vec.size() are equal
        size_t* data = vec.data();
        while( repeat-- )
        {   // sort a vector that is not in order
            for(size_t i = 0; i < size; ++i)
                data[i] = (size - i) % 21;
            std::sort(vec.begin(), vec.end());
        }
    }
    void test_ptr(size_t size, size_t repeat)
    {   // size and vec.size() are equal
        size_t* data = vec.data();
        while( repeat-- )
        {   // sort same vector as in test_itr
            for(size_t i = 0; i < size; ++i)
                data[i] = (size - i) % 21;
            std::sort(vec.data(), vec.data() + vec.size());
        }
    }
}
int main(void)
{   bool ok = true;
    using CppAD::time_test;
    using std::cout;
    //
    size_t test_size = 100000; // size of vector in test
    double time_min  = 1.0;    // minimum time in seconds for each test
    vec.resize(test_size);     // allocate memory outsize of test
    //
    // run the iterator test
    size_t repeat_itr;
    double time_itr  = time_test(test_itr, time_min, test_size, repeat_itr);
    for(size_t i = 1; i < test_size; ++i)
        ok &= vec[i-1] <= vec[i];
    cout << "time_itr=" << time_itr << ", repeat_itr=" << repeat_itr << "\n";
    //
    // run the pointer test
    size_t repeat_ptr;
    double time_ptr  = time_test(test_ptr, time_min, test_size, repeat_ptr);
    for(size_t i = 1; i < test_size; ++i)
        ok &= vec[i-1] <= vec[i];
    cout << "time_ptr=" << time_ptr << ", repeat_ptr=" << repeat_ptr << "\n";
    //
    if( ok )
        return 0;
    return 1;
}
EOF
cxx_flags='-Wall -pedantic-errors -std=c++11 -Wshadow -Wconversion -DNDEBUG -O3'
echo "g++ -I../../include $cxx_flags $name.cpp -o $name"
g++ -I../../include $cxx_flags $name.cpp -o $name
#
echo "build/bug/$name"
if ! ./$name
then
    echo
    echo "build/bug/$name: Error"
    exit 1
fi
echo
# -----------------------------------------------------------------------------
echo "bug/$name.sh: OK"
exit 0
