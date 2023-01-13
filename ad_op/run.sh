# /usr/bin/env bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2023-23 Bradley M. Bell
# -------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
flags="-Wall -pedantic-errors -Wshadow -Wfloat-conversion -Wconversion"
echo_eval g++ $flags -g -I ../include main.cpp -o main
echo_eval ./main
echo_eval rm ./main
echo 'run.sh: OK'
