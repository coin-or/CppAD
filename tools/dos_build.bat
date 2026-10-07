echo off
rem SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
rem SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
rem SPDX-FileContributor: 2024-26 Bradley M. Bell
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
cmd /c tools\dos_build.bat
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
    pause
)
rem
rem CONDA_PREFIX
if defined CONDA_PREFIX (
    echo CONDA_PREFIX = %CONDA_PREFIX%
) else (
    echo CONDA_PREFIX is not defined
    pause
)
rem
rem repo_directory
set repo_directory=%cd%
set cppad_prefix=%repo_directory:\=/%/build/prefix
rem
rem PKG_CONFIG_PATH
set PKG_CONFIG_PATH=%CONDA_PREFIX%\Library\share\pkgconfig
echo PKG_CONFIG_PATH=%PKG_CONFIG_PATH%
if exist %PKG_CONFIG_PATH%\eigen3.pc (
    echo Found eigen3 in PKG_CONFIG_PATH
) else (
    echo Did not find eigen3 in PKG_CONFIG_PATH: suggest
    echo conda install eigen
    pause
)
rem INCLUDE_DIR
set INCLUDE_DIR=%CONDA_PREFIX%\Library\include
echo INCLUDE_DIR=%INCLUDE_DIR%
if exist %INCLUDE_DIR%\Eigen\Core (
    echo Found Eigen\Core in INCLUDE_DIR
) else (
    echo Did not find Eigen\Core in INCLUDE_DIR: suggest
    echo mklink /d %INCLUDE_DIR%\Eigen %INCLUDE_DIR%\eigen3\Eigen
    pause
)
rem MSVS_DIR
set MSVS_DIR=C:\Program Files\Microsoft Visual Studio
echo MSVS_DIR=%MSVS_DIR%
if exist "%MSVS_DIR%" (
    echo Found MSVS_DIR
) else (
    echo Did not find MSVS_DIR
    echo Install Visual Studio ?
    pause
)
rem
rem temp.out
cd  %MSVS_DIR%
dir /s vcvarsall.bat > %repo_directory%/temp.out
cd %repo_directory%
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
    echo Perhaps need to run conda install python
    pause
)
if defined VCINSTALLDIR (
    echo vcvarsall.bat has already been run
) else (
    echo "%VCVARSALL_DIR%\vcvarsall.bat" amd64
    call "%VCVARSALL_DIR%\vcvarsall.bat" amd64
)
rem
rem build
if not exist build ( mkdir build )
cd build
if exist CMakeCache.txt ( rm CMakeCache.txt )
if exist test_install ( rmdir /s /q test_install )
rem
echo cmake
cmake ^
    -B . ^
    -S .. ^
    -G "NMake Makefiles" ^
    -D CMAKE_CXX_COMPILER=cl ^
    -D CMAKE_C_COMPILER=cl ^
    -D CMAKE_BUILD_TYPE=release ^
    -D cmake_install_libdirs=lib ^
    -D cppad_static_lib=false ^
    -D cppad_cxx_flags="/MP /EHs /EHc /std:c++17 /Zc:__cplusplus" ^
    -D cppad_prefix="%cppad_prefix%"
if %ERRORLEVEL% NEQ 0 (
    echo tools/dos_build.bat: cmake command failed
    cd ..
    exit /b %ERRORLEVEL%
)
rem
rem
rem
rem PATH
rem needed to link cppad_lib dll during testing
echo %PATH% | findstr /i %repo_directory%\build\cppad_lib > nul || ^
set PATH=%PATH%;%repo_directory%\build\cppad_lib
rem
rem check
cmake --build . --target check
if %ERRORLEVEL% NEQ 0 (
    echo tools/dos_build.bat: check failed
    cd ..
    exit /b %ERRORLEVEL%
)
rem
rem install
cmake --build . --target install
if %ERRORLEVEL% NEQ 0 (
    echo tools/dos_build.bat: install failed
    cd ..
    exit /b %ERRORLEVEL%
)
rem
rem
rem test_install
mkdir test_install
cd test_install
copy ..\..\example\get_started\get_started.cpp get_started.cpp
cl /Ehsc get_started.cpp ^
    /I %CONDA_PREFIX%\Library\include ^
    /I %cppad_prefix%\include ^
    /link %cppad_prefix%\lib\cppad_lib.lib
if %ERRORLEVEL% NEQ 0 (
    echo tools/dos_build.bat: test of install: failed to build get_started.exe
    cd ..\..
    exit /b %ERRORLEVEL%
)
get_started.exe
if %ERRORLEVEL% NEQ 0 (
    echo tools/dos_build.bat: test of install: get_started.exe failed
    cd ..\..
    exit /b %ERRORLEVEL%
)
echo tools/dos_build.bat: OK
cd ..\..
rem
rem END SOURCE
