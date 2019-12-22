# Plan for batch Edit:
# 1. Remove tab from copyright message.
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
#   example/general/reverse_checkpoint.cpp
#   example/general/runge45_2.cpp
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|/reverse_checkpoint.cpp|/rev_checkpoint.cpp|
#   s|/runge45_2.cpp|/runge_45.cpp|
# '
# list of files that get edited by the extra_seds command
# extra_files='
#   example/general/general.cpp
#   example/general/rev_checkpoint.cpp
#   example/general/rosen_34.cpp
#   example/general/runge_45.cpp
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# extra_seds='
#   s/runge_45_2/runge_45/g
#   s/runge_45_2\(.\)@s/runge_45\1@s@s@s/g
#   s/Rosen34\(.\)@s@s/rosen_34\1@s/g
#   s/Rosen34/rosen_34/g
#   s/reverse_any\(.\)@s@s@s@s/rev_checkpoint\1@s/g
#   s/reverse_any/rev_checkpont/g
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|LuVecADOk\(.\)    |lu_vec_ad_ok\1 |g
s|LuVecADOk|lu_vec_ad_ok|g
s|LuVecAD|lu_vec_ad|g
#
s|extern bool reverse_any(void);|extern bool rev_checkpoint(void);|
#
s|reverse_checkpoint\.cpp|rev_checkpoint.cpp|
s|runge45_2\.cpp|runge_45.cpp|
