echo off 
rem ---------------------------------------------------------------------------
rem CppAD: Copyright (C) 2003-08 Bradley M. Bell
rem
rem CppAD is distributed under multiple licenses. This distribution is under
rem the terms of the 
rem                     Common Public License Version 1.0.
rem
rem A copy of this license is included in the COPYING file of this distribution.
rem visit http://www.coin-or.org/CppAD/ for information on other licenses.
rem ---------------------------------------------------------------------------
goto SKIP_COMMENT
$begin ipopt_cppad_windows$$
$spell
	CppAD
	tarball
	cppad
	yyyymmdd
	ipopt_cppad.sln
$$
$escape $$ $comment so that backslash can appear in codei commands$$

$section Linking the CppAD Interface to Ipopt in Visual Studio 9.0$$

$head Purpose$$
In the special case where you are using Visual Studio 9.0,
you do not need to build Ipopt. 
You can instead follow these instructions
to install the Coin binary distribution for the Ipopt libraries
(where Visual Studio can find them).

$list number$$
Download the binary file below which contains a build of all
most of the Coin-Or projects as of 2008-09-28
$href%
http://www.coin-or.org/download/binary/CoinAll/CoinAll-1.2-VisualStudio.zip%
CoinAll-1.2-VisualStudio.zip
%$$

$lnext
Choose a directory and unzip the file
$code CoinAll-1.2-VisualStudio.zip$$ in that directory.
We refer to this as the $icode from_directory$$ below.

$lnext
Open a Dos shell window and change into your CppAD distribution directory;
e.g., if you install from a tarball, this will be $codei%cppad-%yyyymmdd%$$ 
where $icode yyyymmdd$$ is the year, month, and date corresponding to 
your version of CppAD.

$lnext
Execute the dos command
$codei%
	ipopt_cppad\ipopt_cppad_windows.bat %from_directory%
%$$

$lnext
In Visual Studio open the project file
$codep
	ipopt_cppad\ipopt_cppad.sln
$$
and build the $code Release$$ version of the project
(the $code Debug$$ version is not supported by the CoinAll binary).

$lnext
In the Dos shell window, execute the command
$codep
	ipopt_cppad\Release\ipopt_cppad
$$
It should generate the following output:
$codep
	Ok:    ipopt_cppad_ode
	Ok:    ipopt_cppad_simple
	Ok:    No memory leak detected
	All 3 tests passed.
$$
$lend

$end
: SKIP_COMMENT
rem ---------------------------------------------------------------------------
rem
echo Dos script for linking ipopt to ipopt_cppad examples
rem
if not exist "%1\VisualStudio-Coinall"  goto ERROR_MESSAGE
if not exist ipopt_cppad                   goto ERROR_MESSAGE
rem
echo "creating ipopt_cppad\include\coin directory"
mkdir ipopt_cppad\include\coin
echo "creating ipopt_cppad\lib directry"
mkdir ipopt_cppad\lib
rem
echo "copying necessary ipopt include files to ipopt_cppad\include\coin directory"
copy %1\VisualStudio-Coinall\include\coin\config_ipopt.h  ipopt_cppad\include\coin
copy %1\VisualStudio-Coinall\include\coin\Ip*.hpp         ipopt_cppad\include\coin
copy %1\VisualStudio-Coinall\include\coin\Ip*.h           ipopt_cppad\include\coin
rem
echo "copying ipopt library to ipopt_cppad\lib directory"
copy %1\VisualStudio-Coinall\lib\libipopt.lib ipopt_cppad\lib
rem
echo "copying ifort libraries to ipopt_cppad\lib directory"
copy %1\VisualStudio-Coinall\lib\ifort\* ipopt_cppad\lib
rem
goto DONE
: ERROR_MESSAGE
echo usage: ipopt_cppad\ipopt_cppad_windows from_directory
echo where from_directory contains the sub-directory VisualStudio-Coinall
echo which in an unzipped version of CoinAll-1.2-VisualStudio.zip
echo which comes from http://www.coin-or.org/download/binary/CoinAll.
echo This command must be executed from CppAD distribution directory.
rem
: DONE
