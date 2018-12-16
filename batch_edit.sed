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
# '
# list of sed commands that maps old file and or directory names to new
# file names (the characters @s gets converted to a single space)
# move_seds='
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
s|$pre\t\$|$pre    $|
#
s|\(^ *\)//\t\t\t|\1//            |
s|\(^ *\)//\t\t|\1//        |
s|\(^ *\)//\t|\1//    |
#
s|\(^ *\)if(\t|\1if( |
s/\(^ *\)||\t/\1||  /
#
s|echo \(["']\)\t\t|echo \1        |
s|echo \(["']\)\t|echo \1    |
