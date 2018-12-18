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
#   include/cppad/core/atomic/atomic_base.hpp
# '
# list of sed commands that maps old file and or directory names to new
# file names (the characters @s gets converted to a single space)
# move_seds='
#   s|atomic_base.hpp|atomic_two.hpp|
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
s|atomic_base[.]hpp|atomic_two.hpp|
s|ATOMIC_BASE_HPP|ATOMIC_TWO_HPP|
s|begin atomic_base\$|begin atomic_two$|
s|begin atomic_base\$|begin atomic_two$|
s|begin atomic_example\$|begin atomic_two_example$|
s|$cref atomic_base\$|$cref atomic_two\$|
s|$cref/\([^/]*\)/atomic_base/|$cref/\1/atomic_two/|
s|atomic_example|atomic_two_example|
s|$cref/atomic_two/glossary/|$cref/atomic/glossary/|
s|$cref/atomic_two/atomic_one/|$cref atomic_one|
