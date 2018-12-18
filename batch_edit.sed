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
#   omh/appendix/whats_new/09.omh
# '
# list of sed commands that are applied to the extra files,
# after the other sed commands in this file
# (the characters @s gets converted to a single space)
# extra_seds='
#   s|\$spell|&\n@s@s@s@snlp|
# '
# ----------------------------------------------------------------------------
# Put other sed commands below here and without # at start of line
s|$cref ipopt_nlp_ode_\([a-z][a-z_]*\).cpp|$code ipopt_nlp_ode_\1.cpp|
s|$cref ipopt_nlp_ode_\([a-z][a-z_]*\).hpp|$code ipopt_nlp_ode_\1.hpp|
s|$cref/\([^/]*\)/cppad_ipopt_nlp/Example/$\$|\1|
s|$cref/\([^/]*\)/ipopt_nlp_[a-z_]*/$\$|\1|
s|$cref/ipopt_cppad_nlp/cppad_ipopt_nlp/$\$|$cref cppad_ipopt_nlp$$|
s|$cref%speed tests%ipopt_ode_speed.cpp%$\$|speed tests|
s|$cref \(ipopt_nlp_[a-z_]*\)$\$|$code \1$$|g
s|$cref \(ipopt_nlp_[a-z_]*\).cpp$\$|$code \1.cpp$$|g
s|$cref ipopt_ode_speed.cpp$\$|$code ipopt_ode_speed.cpp$$|g
