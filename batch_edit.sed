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
#	include/cppad/core/checkpoint
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s (@d) gets converted to a space (a dollar sign).
# move_seds='
#	s|/core/checkpoint|/core/chkpoint_one|
# '
# list of files that get edited by the extra_seds command
# extra_files='
#	include/cppad/core/chkpoint_one/checkpoint.hpp
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# extra_seds='
#	s|@dspell@d|&@n@s@s@s@schkpoint|
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|/core/checkpoint/|/core/chkpoint_one/|
s|_CORE_CHECKPOINT_|_CORE_CHKPOINT_ONE_|
