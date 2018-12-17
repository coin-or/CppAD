# ----------------------------------------------------------------------------
# None of the lists below can have white space in an entry.
#
# list of directories that are added to the repository by batch_edit.sh
# new_directories='
# '
# list of files that are deleted by batch_edit.sh
# delete_files='
# '
# list of files that are not edited by the sed commands in this file
# (with the possible exception of the extra_seds commands)
# ignore_files='
# '
# list of files and or directories that are moved to new names
# move_paths='
#   include/cppad/core/atomic_base
#   include/cppad/core/old_atomic.hpp
# '
# list of sed commands that maps old file and or directory names to new
# file names (the characters @s gets converted to a single space)
# move_seds='
#   s|/atomic_base|/atomic|
#   s|/old_atomic[.]hpp|/atomic/atomic_one.hpp|
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
s|_ATOMIC_BASE_|_ATOMIC_|
s|_OLD_ATOMIC_HPP|_ATOMIC_ATOMIC_ONE_HPP|
#
s|/atomic_base/|/atomic/|g
s|file atomic_base/|file atomic/|
#
s|$cref\(.*\)/atomic/|$cref\1/atomic_base/|g
s|/Global Options/atomic_base/|/Global Options/atomic/|g
#
s|/old_atomic[.]hpp|/atomic/atomic_one.hpp|g
s|old_atomic|atomic_one|g
