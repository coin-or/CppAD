#! /bin/bash
#
program="check_svn_dist.sh"
repository="https://projects.coin-or.org/svn/CppAD"
branch=`pwd | sed -e 's|.*/CppAD/||'`
#! /bin/bash
if [ -e "svn_dist" ]
then
	echo "rm -rf svn_dist"
	if ! rm -rf svn_dist
	then
		echo "$program: cannot remove old svn_dist directory"
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
	echo "$program: cannot change into svn_dist directory"
	exit 1
fi
#
echo "./build.sh conf_test"
if ! ./build.sh config_test
then
	echo "$program: error durring ./build.sh config_test in ./svn_dist"
	exit 1
fi
# ----------------------------------------------------------------------------
dir=`pwd`
for name in example test_more ipopt_cppad
do
	echo "cd $name"
	if ! cd $name
	then
		echo "$program: cannot change into $dir/$name directory"
		exit 1
	fi
	echo "make"
	if ! make
	then
		echo "$program: $dir/$name/make failed"
		exit 1
	fi
	echo "./$name"
	if ! ./$name
	then
		echo "$program: $dir/$name/$name failed"
		exit 1
	fi
	echo "cd .."
	cd ..
done
