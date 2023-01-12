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
echo_eval g++ -g -I ../include op_class.cpp -o op_class
echo_eval ./op_class
echo 'ru.sh: OK'
