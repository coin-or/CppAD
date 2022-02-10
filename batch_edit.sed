# Plan for batch Edit:
# 2. Fix following spelling errors:
#    comparision->comparison
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
# '
# list of files and or directories that are moved to new names
# move_paths='
#   example/atomic_four/atomic_vector/
#   include/cppad/example/atomic_four/atomic_vector/
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|atomic_vector|vector|
# '
# list of files that get edited by the extra_seds command
# extra_files='
#   include/cppad/example/atomic_four/atomic_vector.hpp
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# extra_seds='
#   s|EXAMPLE_ATOMIC_FOUR_VECTOR_|EXAMPLE_ATOMIC_FOUR_ATOMIC_VECTOR_|
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of linei
s|atomic_vector_\([a-z_]*\).cpp|atomic_four_vector_\1.cpp|
s|example/atomic_four/atomic_vector/|example/atomic_four/vector/|
s|EXAMPLE_ATOMIC_FOUR_ATOMIC_VECTOR_|EXAMPLE_ATOMIC_FOUR_VECTOR_|
s|ADD_SUBDIRECTORY(atomic_vector)|ADD_SUBDIRECTORY(vector)|
