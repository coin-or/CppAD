#! /bin/bash
#
script="check_svn_dist.sh"
repository="https://projects.coin-or.org/svn/CppAD"
branch=`pwd | sed -e 's|.*/CppAD/||' -e 's|.*/cppad/||'`
#! /bin/bash
if [ -e "svn_dist" ]
then
	echo "rm -rf svn_dist"
	if ! rm -rf svn_dist
	then
		echo "$script: cannot remove old svn_dist directory"
		exit 1
	fi
fi
# ----------------------------------------------------------------------------
echo "svn checkout $repository/$branch svn_dist"
svn checkout $repository/$branch svn_dist
# ----------------------------------------------------------------------------
echo "cd svn_dist"
if ! cd svn_dist
then
	echo "$script: cannot change into svn_dist directory"
	exit 1
fi
#
echo "./build.sh configure"
if ! ./build.sh configure
then
	echo "$script: error durring ./build.sh configure in ./svn_dist"
	exit 1
fi
# ----------------------------------------------------------------------------
# Build and run all the tests
# gcc 3.4.4 with optimization generates incorrect warning; see 
#	http://cygwin.com/ml/cygwin-apps/2005-06/msg00161.html
# The sed commands below are intended to remove them.
dir=`pwd`
echo "make test >& $dir/make.log"
echo "The following will give an overview of progress of command above"
echo "	cat $dir/test.log"
echo "The following will give details of progress of command above"
echo "	tail -f $dir/make.log"
if ! make test >&  make.log
then
	echo "There are errors in $dir/make.log"
	exit 1
fi
if grep 'warning:' make.log
then
	tmp=`pwd`
	echo "Stopping because there are unexpected warnings in"
	echo "$dir/make.log"
	exit 1
fi
echo "OK: make test" 
#
echo "openmp/run.sh"
openmp/run.sh 
