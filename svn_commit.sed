# Changs to be made at each commit
#
# Make sure config.h has certian default values (Used by Microsoft install)
s/^# *define *CPPAD_BOOSTVECTOR *1 *$/#define CPPAD_BOOSTVECTOR 0/
s/^# *define *CPPAD_CPPADVECTOR *0 *$/#define CPPAD_CPPADVECTOR 1/
s/^# *define *CPPAD_STDVECTOR *1 *$/#define CPPAD_STDVECTOR 0/
#
# Delete short copyright comment (it is obvious)
/BEGIN SHORT COPYRIGHT/d
/END SHORT COPYRIGHT/d
#
# improve certain wording
s/an example and a test/an example and test/
#
# Delete statement of where the file is (it is obvious)
/^\$comment This file is in the [^$]*directory\$\$ *$/d
#
# Change copyright second year to current year
s/Copyright (C) \([0-9]*\)-0[0-7] Bradley M. Bell/Copyright (C) \1-09 Bradley M. Bell/
s/Copyright (C) \([0-9]*\)-200[0-7] Bradley M. Bell/Copyright (C) \1-2009 Bradley M. Bell/
