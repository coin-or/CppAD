#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Bradley M. Bell has given permission to use this script to generate
# a distribution of CppAD that has "GNU General Public License Version 3"
# in place of "Eclipse Public License Version 1.0.". Other's are free to
# keep or change this premission in their versions of the source code.
# -----------------------------------------------------------------------------
if [ "$0" != "bin/package.sh" ]
then
	echo "bin/package.sh: must be executed from its parent directory"
	exit 1
fi
echo_eval() {
     echo $*
     eval $*
}
# -----------------------------------------------------------------------------
src_dir=`pwd`
version=`version.sh get`
# -----------------------------------------------------------------------------
# doc
echo 'create ./doc'
if [ -e 'doc' ]
then
	echo_eval rm -r doc
fi
cat << EOF > $src_dir/package.$$
/^commit/! b end
N
N
N
N
/version $version/! b end
s|\\nAuthor:.*||
s|commit *||
p
: end
EOF
#
# use gh-pages if they exist for this version
git_hash=`git log origin/gh-pages | sed -n -f $src_dir/package.$$ | head -1`
if [ "$git_hash" != '' ]
then
	mkdir doc
	list=`git ls-tree --name-only $git_hash:doc`
	for file in $list
	do
		git show $git_hash:doc/$file > doc/$file
	done
else
	# no gh-pages so build documentation (requires omhelp)
	run_omhelp.sh doc
fi
# -----------------------------------------------------------------------------
# change_list
cat << EOF > $src_dir/package.$$
/^.gitignore\$/d
/^authors\$/d
/^bin\\/colpack.sh\$/d
/^coin.png\$/d
/^compile\$/d
/^config.guess\$/d
/^config.sub\$/d
/^configure\$/d
/^depcomp\$/d
/^epl-v10.html\$/d
/^epl-v10.txt\$/d
/^gpl-3.0.txt\$/d
/^install-sh\$/d
/^missing\$/d
/^uw_copy_040507.html\$/d
EOF
change_list=`git ls-files | sed -f $src_dir/package.$$`
# ----------------------------------------------------------------------------
# clean up old results
if [ ! -e 'build' ]
then
	echo_eval mkdir build
fi
echo_eval rm -rf build/cppad-*
echo_eval mkdir build/cppad-$version
# -----------------------------------------------------------------------------
# cppad-$version.tgz
git ls-files -z | xargs -0 tar -czvf build/cppad-$version/tar.tgz
# -----------------------------------------------------------------------------
# cppad-$version and cppad-$version.epl.tgz
echo "create build/cppad-$version.epl.tgz"
cd build/cppad-$version
tar -xzf tar.tgz
rm tar.tgz
rm gpl-3.0.txt
cp -r ../../doc doc
cd ..
tar --create cppad-$version --gzip --file=cppad-$version.epl.tgz
# -----------------------------------------------------------------------------
# cppad-$version and cppad-$version.gpl.tgz
echo "create build/cppad-$version.gpl.tgz"
#
# restore gpl file
cp ../gpl-3.0.txt cppad-$version/gpl-3.0.txt
#
# change EPL to GPL
cat << EOF > $src_dir/package.$$
s|Eclipse Public License Version 1.0|GNU General Public License Version 3|
EOF
#
cd cppad-$version
#
# remove epl files
for file in epl-v10.html epl-v10.txt
do
	rm $file
done
#
# change license in COPYING
sed -i COPYING -e '12,$d'
cat gpl-3.0.txt >> COPYING
#
# Change short copyright message at top of every file
for file in $change_list
do
	sed -i $file -f $src_dir/package.$$
	if ! grep 'GNU General Public License Version 3' $file > /dev/null
	then
		echo "package.sh: Cannot change $file"
	fi
done
rm $src_dir/package.$$
#
# cppad-$version.gpl.tgz
cd ..
tar --create cppad-$version --gzip --file=cppad-$version.gpl.tgz
# ----------------------------------------------------------------------------
# remove cppad-$version
rm -r cppad-$version
#
echo 'package.sh: OK'
exit 0
