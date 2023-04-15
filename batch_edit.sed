# ----------------------------------------------------------------------------
# None of the lists below can have white space or a dollar sign in an entry.
#
# list of directories that are added to the repository by batch_edit.sh
# new_directories='
# '
# list of files that are deleted by batch_edit.sh
# delete_files='
# '
# List of files that are not edited by the sed commands in this file
# (with the possible exception of the extra_seds commands).
# The files in bin/devel.sh ignore_files are automatically in this list.
# ignore_files='
#  val_graph/comp_xam.cpp
#  val_graph/test/fun2val.cpp
#  include/cppad/local/val_graph/tape.hpp
# '
# list of files and or directories that are moved to new names
# move_paths='
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
# '
# list of files that get edited by the extra_seds command
# extra_files='
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# extra_seds='
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of each line
/size_t& *compare_false *,/! b one
N
s|size_t&       |SIZE_T\&|
s|Vector<Value>&|size_t\&       |
s|SIZE_T&|Vector<Value>\&|
#
s|val_vec      |VAL_VEC|
s|val_vec$|VAL_VEC|
s|compare_false|val_vec      |
s|VAL_VEC|compare_false|
s|compare_false *$|compare_false|
: one
