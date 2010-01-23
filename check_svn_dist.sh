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
echo "./build.sh conf_test"
if ! ./build.sh config_test
then
	echo "$script: error durring ./build.sh config_test in ./svn_dist"
	exit 1
fi
# ----------------------------------------------------------------------------
dir=`pwd`
for name in example test_more cppad_ipopt/example cppad_ipopt/speed
do
	echo "cd $dir/$name"
	if ! cd $dir/$name
	then
		echo "$script: cannot change into $dir/$name directory"
		exit 1
	fi
	echo "make"
	if ! make
	then
		echo "$script: $dir/$name/make failed"
		exit 1
	fi
	program=`echo $name | sed -e 's|.*/||'`
	echo "./$program"
	if ! ./$program
	then
		echo "$script: $dir/$name/$program failed"
		exit 1
	fi
	echo "cd $dir"
	cd $dir
done
