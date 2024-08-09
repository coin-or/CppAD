echo off
rem --------------------------------------------------------------------------
goto end_of_comment_block
{xrst_begin dos_build.bat}
{xrst_spell
   cmd
}

Compile and Test CppAD using Dos
################################

Syntax
******
{xrst_code bat}
cmd /c bin\dos_build.bat
{xrst_code}

Eigen
*****
This example includes the eigen optional package.
Other optional packages could be included in a similar fashion.

Source
******
{xrst_literal 
   rem BEGIN SOURCE
   rem END SOURCE
}

{xrst_end dos_build.bat}
:end_of_comment_block
rem ---------------------------------------------------------------------------
rem BEGIN SOURCE
rem .git
if not exist .git (
   echo Expected .git to be a subdirectory of working directory
   exit 1
)
rem
rem CONDA_PREFIX
if defined CONDA_PREFIX (
   echo CONDA_PREFIX = %CONDA_PREFIX%
) else (
   echo CONDA_PREFIX is not defined
   exit
)
rem PKG_CONFIG_PATH
set PKG_CONFIG_PATH=%CONDA_PREFIX%\Library\share\pkgconfig
echo PKG_CONFIG_PATH=%PKG_CONFIG_PATH%
if exist %PKG_CONFIG_PATH%\eigen3.pc (
   echo Found eigen3 in PKG_CONFIG_PATH
) else (
   echo Did not find eigen3 in PKG_CONFIG_PATH: suggest
   echo conda install eigen
   exit
)
rem INCLUDE_DIR
set INCLUDE_DIR=%CONDA_PREFIX%\Library\include
echo INCLUDE_DIR=%INCLUDE_DIR%
if exist %INCLUDE_DIR%\Eigen\Core (
   echo Found Eigen\Core in INCLUDE_DIR
) else (
   echo Did not find Eigen\Core in INCLUDE_DIR: suggest
   echo mklink /d %INCLUDE_DIR%\Eigen %INCLUDE_DIR%\eigen3\Eigen
   exit
)
rem MSVS_DIR
set MSVS_DIR=C:\Program Files\Microsoft Visual Studio
echo MSVS_DIR=%MSVS_DIR%
if exist "%MSVS_DIR%" (
   echo Found MSVS_DIR
) else (
   echo Did not find MSVS_DIR
   echo Install Visual Studio ?
   exit 1
)
rem
rem temp.out
set SAVE_DIR=%cd%
cd  %MSVS_DIR%
dir /s vcvarsall.bat > %SAVE_DIR%/temp.out
cd %SAVE_DIR%
ren
rem temp.py
echo import re                            > temp.py
echo f_obj   = open('temp.out', 'r')      >> temp.py
echo data    = f_obj.read()               >> temp.py
echo pattern = '\n *Directory *of *(.*)'  >> temp.py
echo m_obj   = re.search(pattern, data)   >> temp.py
echo if m_obj == None :                   >> temp.py
echo    print( 'not_found' )              >> temp.py
echo else :                               >> temp.py
echo    print( m_obj.group(1) )           >> temp.py
rem rem
rem VCVARSALL_DIR
python temp.py > temp
set /p VCVARSALL_DIR=<temp
echo VCVARSALL_DIR = %VCVARSALL_DIR%
if exist "%VCVARSALL_DIR%\vcvarsall.bat" ( 
   echo Found vcvarsall.bat in VCVARSALL_DIR
) else (
   echo Could not find vcvarsall.bat below MSVS_DIR
   exit
)
rem vcvarsall.bat
call "%VCVARSALL_DIR%\vcvarsall.bat" amd64
rem
rem build
if not exist build ( mkdir build )
cd build
if exist CMakeCache.txt ( rm CMakeCache.txt )
rem
rem cmakde
cmake ^
   -B . ^
   -S .. ^
   -G "NMake Makefiles" ^
   -D CMAKE_CXX_COMPILER=cl ^
   -D CMAKE_C_COMPILER=cl ^
   -D CMAKE_BUILD_TYPE=release ^
   -D cppad_static_lib=true ^
   -D cppad_cxx_flags="/MP /EHs /EHc /std:c++17 /Zc:__cplusplus"
rem
rem check
cmake --build . --target check
rem
echo bin/dos_build.bat: OK
rem END SOURCE
