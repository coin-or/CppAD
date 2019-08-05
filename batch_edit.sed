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
#   example/multi_thread/multi_atomic.cpp
#   example/multi_thread/multi_atomic.hpp
#   example/multi_thread/multi_atomic.omh
#   test_more/cppad_for_tmb/multi_atomic.cpp
#   example/multi_thread/multi_checkpoint.cpp
#   example/multi_thread/multi_checkpoint.hpp
#   example/multi_thread/multi_checkpoint.omh
#   test_more/cppad_for_tmb/multi_checkpoint.cpp
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|multi_atomic|multi_atomic_two|
#   s|multi_checkpoint|multi_chkpoint_one|
# '
# list of files that get edited by the extra_seds command
# extra_files='
#   omh/appendix/whats_new/18.omh
#   example/multi_thread/multi_chkpoint_one.omh
#   example/multi_thread/multi_chkpoint_one.cpp
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# extra_seds='
#   s|@dspell|&@n@s@s@s@schkpoint|
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|MULTI_ATOMIC|MULTI_ATOMIC_TWO|
s|multi_atomic|multi_atomic_two|g
#
s|MULTI_CHECKPOINT|MULTI_CHKPOINT_ONE|
s|multi_checkpoint|multi_chkpoint_one|g

