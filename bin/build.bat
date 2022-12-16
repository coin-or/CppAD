rmdir /S build
mkdir build
cd build
cmake ^
   -G "NMake Makefiles"^
   -D cppad_cxx_flags="/MP /EHs /EHc /std:c++17 /Zc:__cplusplus"^
   ..
nmake check
