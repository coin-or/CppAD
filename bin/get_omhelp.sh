#! /bin/bash -e
# Copyright: None
# -----------------------------------------------------------------------------
web_page='https://github.com/bradbell/omhelp.git'
version='20190202'
git_hash='82244e99d9d523aaac5f9a6fe3817f52c13b8a60'
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
echo_eval mkdir build
echo_eval cd build
echo_eval cmake \
    -D boost_regex_prefix='/usr' \
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
