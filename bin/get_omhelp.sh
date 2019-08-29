#! /bin/bash -e
# Copyright: None
# -----------------------------------------------------------------------------
web_page='https://github.com/bradbell/omhelp.git'
version='20190829'
git_hash='45353cb7853643277e2b57a20f8ddd22d017e4e6'
start_dir=`pwd`
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
name='omhelp'
if [ "$0" != "bin/get_$name.sh" ]
then
    echo "get_$name.sh should be in the ./bin directory and executed using" 
    echo "bin/get_$name.sh"
    exit 1
fi
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
    mkdir build
fi
cd build
# -----------------------------------------------------------------------------
if [ ! -e omhelp.git ]
then
    git clone $web_page omhelp.git
fi
# -----------------------------------------------------------------------------
cd omhelp.git
git checkout master
git pull
git checkout --quiet $git_hash
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
    mkdir build
fi
echo_eval cd build
echo_eval cmake \
    -D source_highlight_prefix="$start_dir/build/prefix" \
    -D omhelp_prefix="$start_dir/build/prefix"  \
    -D omhelp_datadir=share \
    -D omhelp_build_type=release \
    .. | tee cmake.log
if grep WARNING cmake.log
then
    echo "get_$name.sh: aborting due to cmake command warnings"
    exit 1
fi
echo_eval make install
# -----------------------------------------------------------------------------
echo "get_$name.sh: OK"
exit 1
