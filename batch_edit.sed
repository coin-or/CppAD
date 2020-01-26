# Plan for batch Edit:
# 1. Remove tab from copyright message.
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
#   speed/omh/speed.omh
#   speed/omh/speed_utility.omh
#   speed/omh/speed_adolc.omh
#   speed/omh/speed_cppad.omh
#   speed/omh/speed_cppadcg.omh
#   speed/omh/speed_double.omh
#   speed/omh/speed_fadbad.omh
#   speed/omh/speed_sacado.omh
#   speed/omh/speed_xpackage.omh
# '
# list of sed commands that map old file and directory names to new names.
# The characters @s, @d, @n get converted to a space, dollar sign, new line.
# move_seds='
#   s|speed/omh/speed.omh|speed/speed.omh|
#   s|speed/omh/speed_utility.omh|speed/speed_utility.omh|
#   s|speed/omh/speed_adolc.omh|speed/adolc/speed_adolc.omh|
#   s|speed/omh/speed_cppad.omh|speed/cppad/speed_cppad.omh|
#   s|speed/omh/speed_cppadcg.omh|speed/cppadcg/speed_cppadcg.omh|
#   s|speed/omh/speed_double.omh|speed/double/speed_double.omh|
#   s|speed/omh/speed_fadbad.omh|speed/fadbad/speed_fadbad.omh|
#   s|speed/omh/speed_sacado.omh|speed/sacado/speed_sacado.omh|
#   s|speed/omh/speed_xpackage.omh|speed/xpackage/speed_xpackage.omh|
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
s|speed/omh/speed.omh|speed/speed.omh|
s|speed/omh/speed_utility.omh|speed/speed_utility.omh|
s|speed/omh/speed_adolc.omh|speed/adolc/speed_adolc.omh|
s|speed/omh/speed_cppad.omh|speed/cppad/speed_cppad.omh|
s|speed/omh/speed_cppadcg.omh|speed/cppadcg/speed_cppadcg.omh|
s|speed/omh/speed_double.omh|speed/double/speed_double.omh|
s|speed/omh/speed_fadbad.omh|speed/fadbad/speed_fadbad.omh|
s|speed/omh/speed_sacado.omh|speed/sacado/speed_sacado.omh|
s|speed/omh/speed_xpackage.omh|speed/xpackage/speed_xpackage.omh|
#
s|speed/omh/speed_$package.omh|speed/$package/speed_$package.omh|
