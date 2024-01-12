rmdir /S build
mkdir build
cd build
cmake ^
   -G "NMake Makefiles"^
   -D cppad_cxx_flags="/MP /EHs /EHc /std:c++17 /Zc:__cplusplus"^
   -D cppad_static_lib=FALSE^
   ..
nmake check
