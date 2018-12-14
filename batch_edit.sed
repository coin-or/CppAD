# ----------------------------------------------------------------------------
# None of the lists below can have white space in an entry.
#
# list of directories that are added to the repository by batch_edit.sh
# new_directories='
# '
# list of files that are deleted by batch_edit.sh
# delete_files='
#   bin/speed_temp.sh
# '
# list of files that are not edited by the sed commands in this file
# (with the possible exception of the extra_seds commands)
# ignore_files='
# '
# list of files and or directories that are moved to new names
# move_paths='
#   include/cppad/core/atomic_base.hpp
#   include/cppad/core/checkpoint.hpp
#   include/cppad/local/player.hpp
# '
# list of sed commands that maps old file and or directory names to new
# file names (the characters @s gets converted to a single space)
# move_seds='
#   s|/atomic_base[.]hpp|/atomic_base/atomic_base.hpp|
#   s|/checkpoint[.]hpp|/checkpoint/checkpoint.hpp|
#   s|/player[.]hpp|/play/player.hpp|
# '
# list of files that get edited by the extra_seds command
# extra_files='
#   include/cppad/core/checkpoint/checkpoint.hpp
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file
# (the characters @s gets converted to a single space)
# extra_seds='
#   s|_CHECKPOINT_HPP|_CHECKPOINT_CHECKPOINT_HPP|
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|/atomic_base[.]hpp|/atomic_base/atomic_base.hpp|
s|_ATOMIC_BASE_HPP|_ATOMIC_BASE_ATOMIC_BASE_HPP|
#
s|/checkpoint[.]hpp|/checkpoint/checkpoint.hpp|
#
s|/player[.]hpp|/play/player.hpp|
s|_PLAYER_HPP|_PLAY_PLAYER_HPP|
