#! /bin/bash -e
temporary_dir="$HOME/trash"
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
# make sure that build.sh is the only file that has changed
list=`git status -s | sed -e '/ build.sh$/d' -e '/ .build.sh.swp$/d' `
if [ "$list" != '' ]
then
	git status -s
	echo 'build.sh: git files other thant build.sh have changed'
	exit 1
fi
# -----------------------------------------------------------------------------
# copy build.sh to temporary directory
echo_eval cp build.sh $temporary_dir/cppad.build.sh
# -----------------------------------------------------------------------------
# checkout master
echo_eval git checkout master
# build documentation
echo_eval bin/run_omhelp.sh htm
# copy documentation to temporary directory
if [ -e $temporary_dir/cppad.doc ]
then
	echo_eval rm -r $temporary_dir/cppad.doc
fi
echo_eval mv doc $temporary_dir/cppad.doc
# -----------------------------------------------------------------------------
# checkout gh-pages
echo_eval git checkout gh-pages
# restore build.sh
echo_eval cp $temporary_dir/cppad.build.sh build.sh
# get temporary directory version of documentation
echo_eval rm -r doc
echo_eval mv $temporary_dir/cppad.doc doc
# need .nojekyll file because doc has files that begin with underbars
echo_eval touch doc/.nojekyll
# change icon link to http://coin-or.github.io/CppAD/
for file in doc/*.htm
do
	sed -i $file -e 's|/www\.coin-or\.org/|/coin-or.github.io/|'
done
# -----------------------------------------------------------------------------
add_list=`git status -s | sed -n -e '/^?? /p' | sed -e 's/^?? *//'`
git add $add_list
# -----------------------------------------------------------------------------
echo 'build.sh: OK'
