#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
if [ ! -e "bin/check_srcfile.sh" ]
then
    echo "bin/check_srcfile.sh: must be executed from its parent directory"
    exit 1
fi
cat << EOF > junk.sed
/\$srcfile[^a-z]/! b skip
N
s/^# *//
s/^ *//
s/\\n# *//
s/\\n *//
s/\$srcfile%//
s/%.*//
p
: skip
EOF
# list of files that reference other files in srcfile commands
special_case='
batch_edit.sed
bin/check_srcfile.sh
include/cppad/core/cond_exp.hpp
introduction/exp_2.omh
introduction/exp_eps.omh
omh/appendix/license.omh
speed/cppadcg/det_minor_grad.c
speed/cppadcg/sparse_jacobian.c
'
# -----------------------------------------------------------------------------
# Make sure that OMhelp srcfile commands refer to same file as command
echo "Checking that OMhelp srcfile commands include from file they appear in."
echo "----------------------------------------------------------------------"
list=`git ls-files`
different="no"
for file in $list
do
    special='no'
    for name in $special_case
    do
        if [ "$file" == "$name" ]
        then
            special='yes'
        fi
    done
    #
    bad_reference=''
    reference_list=`sed -n -f junk.sed $file`
    if [ "$special" == 'no' ] && [ "$reference_list" != '' ]
    then
        for reference in $reference_list
        do
            if [ "$reference" != "$file" ]
            then
                bad_reference="$reference"
            fi
        done
    fi
    #
    ext=`echo $file | sed -e 's|.*\.||'`
    if [ "$bad_reference" != '' ] && [ "$ext" == 'omh' ]
    then
        file_root=`echo $file | sed -e 's|.*/||' -e 's|_hpp\.omh|.hpp|'`
        ref_root=`echo $reference | sed -e 's|.*/||'`
        if [ "$file_root" == "$ref_root" ]
        then
            bad_reference=''
        fi
        file_root=`echo $file | sed -e 's|.*/||' -e 's|\.omh|.hpp|'`
        if [ "$file_root" == "$ref_root" ]
        then
            bad_reference=''
        fi
    fi
    #
    if [ "$bad_reference" != '' ]
    then
        echo "\$srcfile in $file references"
        echo "$bad_reference"
        different="yes"
    fi
done
echo "-------------------------------------------------------------------"
if [ $different = "yes" ]
then
    echo "Error: nothing should be between the two dashed lines above"
    exit 1
else
    echo "OK: nothing is between the two dashed lines above"
    exit 0
fi
