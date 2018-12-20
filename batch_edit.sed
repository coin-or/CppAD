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
#   include/cppad/local/ad_type.hpp
# '
# list of sed commands that maps old file and or directory names to new
# file names (the characters @s gets converted to a single space)
# move_seds='
#   s|/local/|/core/|
# '
# '
# list of files that get edited by the extra_seds command
# extra_files='
#   include/cppad/core/ad_type.hpp
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file
# (the characters @s gets converted to a single space)
# extra_seds='
#   s|namespace@slocal@s{@s//@sBEGIN_CPPAD_LOCAL|//@sBEGIN_CPPAD|
#   s|}@s}@s//@sEND_CPPAD_LOCAL|}@s//@sEND_CPPAD|
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|/local/ad_type[.]hpp|/core/ad_type.hpp|
s|local::ad_type_enum|ad_type_enum|g
s|local::constant_enum|constant_enum|g
s|local::dynamic_enum|dynamic_enum|g
s|local::variable_enum|variable_enum|g
