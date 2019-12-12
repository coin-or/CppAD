# Plan for batch Edit:
# 1. Remove tab from copyright message.
# 2. Change mode of all *.omh files to -x.
# ----------------------------------------------------------------------------
# None of the lists below can have white space or a dollar sign in an entry.
#
# list of directories that are added to the repository by batch_edit.sh
# new_directories='
#   include/cppad/core/forward
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
#   omh/forward
#   include/cppad/core/forward.hpp
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|omh/|include/cppad/core/|
#   s|forward.hpp|/forward/forward.hpp|
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
# Put other sed commands below here and without # at start of line
s|omh/forward/|include/cppad/core/forward/|
s|cppad/core/forward.hpp|cppad/core/forward/forward.hpp|
s|CPPAD_CORE_FORWARD_HPP|CPPAD_CORE_FORWARD_FORWARD_HPP|
