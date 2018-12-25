# ----------------------------------------------------------------------------
# None of the lists below can have white space in an entry.
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
#   omh/appendix/whats_new/16.omh
#   omh/appendix/whats_new/15.omh
# '
# list of files and or directories that are moved to new names
# move_paths='
#   example/atomic/atomic.cpp
#   example/atomic
# '
# list of sed commands that maps old file and or directory names to new 
# file names (the characters @s gets converted to a single space)
# move_seds='
#   s|atomic[.]cpp|atomic_two.cpp|
#   s|example/atomic$|example/atomic_two|
# '
# list of files that get edited by the extra_seds command
# extra_files='
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file
# (the characters @s gets converted to a single space)
# extra_seds='
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|example/atomic/|example/atomic_two/|g
s|^\texample/atomic \\|\texample/atomic_two \\|
s|example_atomic|example_atomic_two|g
s|ADD_SUBDIRECTORY(atomic)|ADD_SUBDIRECTORY(atomic_two)|
s|\([^a-zA-Z_]\)atomic.cpp|\1atomic_two.cpp|
