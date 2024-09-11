#! /usr/bin/env bash
set -e -u
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2023-24 Bradley M. Bell
# ----------------------------------------------------------------------------
# fix sort order; see
# unix.stackexchange.com/questions/87745/what-does-lc-all-c-do/87763#87763
export LC_ALL='C'
#
if [ "$#" == 0 ]
then
cat<< EOF
usage: sort.sh file_name
BEGIN_SORT_THIS_LINE_PLUS_nb: defines beginning line for sort.
END_SORT_THIS_LINE_MINUS_ne:  defines ending line for sort.
The tokens nb and ne are both a single non-zero decimal digit.

If the exit status is 0, the last line on standard out is one of the following:
'sort.sh: OK'    The file was already sorted
'sort.sh: Done'  The file was sorted
EOF
   exit 1
fi
file_name="$1"
#
# grep, sed
source bin/grep_and_sed.sh
#
if $grep 'BEGIN_SORT_THIS_LINE_PLUS_[1-9][0-9]' "$file_name"
then
   echo "in BEGIN_SORT_THIS_LINE_PLUS_nb in file $file_name"
   echo 'nb has more that one decial digit.'
   exit 1
fi
if $grep 'END_SORT_THIS_LINE_MINUS_[1-9][0-9]' "$file_name"
then
   echo "in END_SORT_THIS_LINE_PLUS_ne in file $file_name"
   echo 'ne has more that one decial digit.'
   exit 1
fi
# --------------------------------------------------------------------------
#
# is_file_executable
if [ -x $file_name ]
then
   is_file_executable='yes'
else
   is_file_executable='no'
fi
#
# begin_sum
# is th sum for the beginning line number
set +e
begin_sum=`$grep --line-number 'BEGIN_SORT_THIS_LINE_PLUS_[1-9]' $file_name | \
   $sed -e 's|\([0-9]*\):.*BEGIN_SORT_THIS_LINE_PLUS_\([1-9]\).*|\1+\2|'`
set -e
if [ "$begin_sum" == '' ]
then
   echo "sort.sh $file_name"
   echo "Cannot find BEGIN_SORT_THIS_LINE_PLUS_nb in $file_name"
   exit 1
fi
#
# begin_line
begin_count=0
for sum in $begin_sum
do
   # This does the summation
   let begin_line[$begin_count]="$sum"
   let begin_count="$begin_count + 1"
done
#
# end_diff
# ios the difference for the ending line number
set +e
end_diff=`$grep --line-number 'END_SORT_THIS_LINE_MINUS_[1-9]' $file_name | \
   $sed -e 's|\([0-9]*\):.*END_SORT_THIS_LINE_MINUS_\([1-9]\).*|\1-\2|'`
set -e
if [ "$end_diff" == '' ]
then
   echo "sort.sh $file_name"
   echo "Cannot find END_SORT_THIS_LINE_MINUS_nb in $file_name"
   exit 1
fi
#
# end_line
end_count=0
for diff in $end_diff
do
   # This does the difference
   let end_line[$end_count]="$diff"
   let end_count="$end_count + 1"
done
if [ $begin_count != $end_count ]
then
   echo "sort.sh $file_name"
   echo 'number of BEGIN_SORT_THIS_LINE_PLUS_nb is not equal to'
   echo 'number of END_SORT_THIS_LINE_MINUS_ne.'
   exit 1
fi
#
# first_line
first_line='1'
#
# last_line
last_line=`wc -l $file_name | $sed -e 's|^ *\([0-9]*\) .*|\1|'`
#
# count, stop_line_previous, sorted.$$
count=0
stop_line_previous=0
cp $file_name sorted.$$
while [ $count -lt $begin_count ]
do
   #
   # start_line, stop_line, count
   start_line=${begin_line[$count]}
   stop_line=${end_line[$count]}
   let count="$count + 1"
   echo "sort.sh: sorting lines $start_line to $stop_line in $file_name"
   #
   if [ $start_line -ge $stop_line ]
   then
      echo "start_line = $start_line >= stop_line = $stop_line "
      exit 1
   fi
   #
   # stop_line_previous
   if [ $stop_line_previous -ge $start_line ]
   then
      echo "previous stop_line=$stop_line_previous >= start_line=$start_line"
      exit 1
   fi
   stop_line_previous="$stop_line"
   #
   # temp.$$
   if [ "$start_line" != "$first_line" ]
   then
      let start_m1="$start_line - 1"
      $sed -n -e "1,${start_m1}p" sorted.$$ >> temp.$$
   fi
   #
   $sed -n -e "${start_line},${stop_line}p" sorted.$$ | sort >> temp.$$
   #
   if [ "$stop_line" != "$last_line" ]
   then
      stop_p1=`expr $stop_line + 1`
      $sed -n -e "${stop_p1},${last_line}p" sorted.$$ >> temp.$$
   fi
   #
   # sorted.$$
   mv temp.$$ sorted.$$
done
if diff sorted.$$ $file_name > /dev/null
then
   rm sorted.$$
   echo 'sort.sh: OK'
else
   mv sorted.$$ $file_name
   if [ "$is_file_executable" == 'yes' ]
   then
      chmod +x $file_name
   fi
   echo 'sort.sh: Done'
fi
exit 0
