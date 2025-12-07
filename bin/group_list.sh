#! /usr/bin/env bash
set -e -u
# !! EDITS TO THIS FILE ARE LOST DURING UPDATES BY xrst.git/bin/dev_tools.sh !!
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2020-25 Bradley M. Bell
# ----------------------------------------------------------------------------
# bin/group_list.sh
# Returns a space separated list of the group names for this project.
# The list is extracted from the [root_file] seciton of the xrst.toml
# file for this project.
# If there is no xrst.toml file for this project, the group list is just
# the default group.
#
# sed
source bin/grep_and_sed.sh
#
if [ ! -e xrst.toml ]
then
   echo 'default'
   exit 0
fi
#
# temp.sed
cat << EOF > temp.sed
/^\\[root_file\\]/ ! b end
: loop
N
/\\n\\[/! b loop
s|^\\[root_file\\]||
s|\\n *#[^\\n]*||g
s|\\n\\([A-Za-z0-9_.]*\\) *=[^\\n]*|\\1 |g
s|\\n[^\\n]*\$||
#
p
#
: end
EOF
#
# group_list
group_list=$( $sed -n -f temp.sed xrst.toml)
#
echo "$group_list"
