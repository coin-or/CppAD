#! /usr/bin/env bash
set -e -u
# !! EDITS TO THIS FILE ARE LOST DURING UPDATES BY xrst.git/tools/dev_tools.sh !!
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2026 Bradley M. Bell
# ----------------------------------------------------------------------------
# script_path
script_dir="$( dirname -- "${BASH_SOURCE[0]}" )"
script_dir="$( cd -- "$script_dir" &> /dev/null && pwd )"
script_path="$script_dir/$(basename $0)"
#
# tools/check_copy.sh
# Checks that the copyright message, in all the source files,
# is correct and up to date. If there were any errors, a message is printed,
# it is automatically corrected, and this script exits with an error.
# Files that are not checked can be specified in tools/dev_setting.sh
# ----------------------------------------------------------------------------
if [ ! -e 'tools/check_copy.sh' ]
then
    echo "tools/check_copy.sh: must be executed from its parent directory"
    exit 1
fi
if [ "$#" != 0 ]
then
    echo 'check_copy does not expect any arguments'
    exit 1
fi
#
# grep, sed
source tools/grep_and_sed.sh
#
# package_name, spdx_license_id, spdx_copyright_text, no_copyright_list
source tools/dev_settings.sh
#
# yy
yy=$(date +%y)
#
# ----------------------------------------------------------------------------
if [ $# != 0 ]
then
    echo 'tools/check_copy.sh does not expect any arguments'
    exit 1
fi
if [ ! -e 'tools/check_copy.sh' ]
then
    echo 'tools/check_copy.sh: must be executed from its parent directory'
    exit 1
fi
if [ ! -e './.git' ]
then
    echo 'tools/check_copy.sh: cannot find ./.git'
    exit 1
fi
# ---------------------------------------------------------------------------
# fullname
fullname=''
if [ "${USER+x}" != '' ]
then
    for contributor in $contributor_list
    do
        if [[ $contributor == ${USER}* ]]
        then
            fullname=$(echo $contributor | sed -e 's|^.*:||' -e 's|_| |g')
        fi
    done
    if [ "$fullname" == '' ] && [ "${USER+x}" != '' ]
    then
        echo "Cannot user name = $USER in tools/dev_settings.sh contributor_list"
        exit 1
    fi
fi
# ---------------------------------------------------------------------------
# copyright_all, copyright_changed
echo "#" > temp.sed
for name in $no_copyright_list
do
    if [ -f $name ]
    then
        echo "^$name\$" | $sed -e 's|/|[/]|g' -e 's|.*|/&/d|' >> temp.sed
    elif [ -d $name ]
    then
        echo "^$name/" | $sed -e 's|/|[/]|g' -e 's|.*|/&/d|' >> temp.sed
    else
        echo "$name in no_copyright_list is not a file or directory"
        exit 1
    fi
done
copyright_all=$(git ls-files | $sed -f temp.sed)
copyright_changed=$(
    git status --porcelain | $sed -e 's|^...||' | $sed -f temp.sed
)
# ---------------------------------------------------------------------------
# missing
missing='no'
for file_name in $copyright_all
do
    # if file has not been deleted
    if [ -e $file_name ]
    then
        # if file does not have expected license identifier
        if ! $grep "$spdx_license_id\$" $file_name > /dev/null
        then
            if [ "$missing" == 'no' ]
            then
                echo "Cannot find line that ends with:"
                echo "   $spdx_license_id"
                echo "In the following files:"
            fi
            echo "$file_name"
            missing='yes'
        fi
    fi
done
if [ "$missing" == 'yes' ]
then
    echo 'check_copy.sh: spdx_license_id is missing'
    exit 1
fi
# ---------------------------------------------------------------------------
# missing
missing='no'
#
# dev_tools
# The copyright text for the development tools does not change
# must be the same as in tools/dev_tools.sh
# BEGIN_SORT_THIS_LINE_PLUS_2
dev_tools='
    .readthedocs.yaml
    tools/check_copy.sh
    tools/check_invisible.sh
    tools/check_sort.sh
    tools/check_tab.sh
    tools/check_version.sh
    tools/dev_settings.sh
    tools/git_commit.sh
    tools/grep_and_sed.sh
    tools/group_list.sh
    tools/new_file.sh
    tools/new_release.sh
    tools/run_xrst.sh
    tools/sort.sh
    tools/tools_path.sh
    tools/twine.sh
'
# END_SORT_THIS_LINE_MINUS_1
for file_name in $copyright_all
do
    check='yes'
    if [ ! -e $file_name ]
    then
        check='no'
    fi
    if [[ "$dev_tools" == *"$file_name"* ]]
    then
        if [ "$package_name" != 'xrst' ]
        then
            check='no'
        fi
    fi
    # if file has not been deleted
    if [ "$check" == 'yes' ]
    then
        # if file does not have expected copyright text
        if ! $grep "$spdx_copyright_text\$" $file_name > /dev/null
        then
            if [ "$missing" == 'no' ]
            then
                echo "Cannot find line that ends with:"
                echo "   $spdx_copyright_text"
                echo "In the following files:"
            fi
            echo "$file_name"
            missing='yes'
        fi
    fi
done
if [ "$missing" == 'yes' ]
then
    echo 'check_copy.sh: spdx_copyright_text is missing'
    exit 1
fi
# ---------------------------------------------------------------------------
# changed
changed='no'
cat << EOF > temp.sed
/SPDX-FileContributor:[ 0-9.-]*$fullname/! b end
s|\\([0-9]\\{4\\}\\)[-0-9]* |\\1-$yy |
s|20$yy-$yy |20$yy |
#
: end
EOF
list=''
if [ "$fullname" != '' ]
then
    list="$copyright_changed"
fi
for file_name in $list
do
    if [ -e $file_name ] && [ -f $file_name ]
    then
        if ! $grep "SPDX-FileContributor:[ 0-9.-]*$fullname" $file_name \
            > /dev/null
        then
            echo "username = $USER, fullname = $fullname"
            echo "The following pattern does not appear in $file_name"
            echo 'SPDX-FileContributor:[ 0-9.-]*'$fullname:
            exit 1
        fi
        $sed -f temp.sed $file_name > temp.$$
        if diff $file_name temp.$$ > /dev/null
        then
            rm temp.$$
        else
            if [ "$changed" == 'no' ]
            then
                echo 'The following file contributor dates have been updated'
            fi
            echo $file_name
            if diff $file_name temp.$$
            then
                echo 'check_version.sh: program error'
                exit 1
            fi
            changed='yes'
            if [ -x $file_name ]
            then
                mv temp.$$ $file_name
                chmod +x $file_name
            else
                mv temp.$$ $file_name
            fi
        fi
    fi
done
#
if [ "$changed" == 'yes' ]
then
    echo 'check_copy.sh: The copyright messages above were updated.'
    echo 'Re-execute tools/check_copy.sh ?'
    exit 1
fi
echo "$script_path: OK"
exit 0
