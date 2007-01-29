# Changs to be made at each commit
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
s/Copyright (C) \([0-9]*\)-0[0-6] Bradley M. Bell/Copyright (C) \1-07 Bradley M. Bell/
s/Copyright (C) \([0-9]*\)-200[0-6] Bradley M. Bell/Copyright (C) \1-2007 Bradley M. Bell/
