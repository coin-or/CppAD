#! /bin/bash -e
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# -----------------------------------------------------------------------------
edit_file()
{  file="$1"
   echo $file
   cp $file ~/trash
   sed -i $file -f temp.sed
   echo '--------------------------------------------------------------------'
   if diff $file ~/trash
   then
      echo "$file: no changes"
   else
      echo "$file: changes above"
   fi
}
# ----------------------------------------------------------------------------
# check_include_xrst.sh
if [ -e bin/check_include_omh.sh ]
then
   git mv bin/check_include_omh.sh bin/check_include_xrst.sh
fi
# ----------------------------------------------------------------------------
cat << EOF > temp.sed
s|omh|xrst|g
EOF
edit_file bin/check_include_xrst.sh
edit_file bin/check_define.sh
edit_file bin/check_example.sh
# ----------------------------------------------------------------------------
cat << EOF > temp.sed
s|"[*].hpp" PATTERN "omh"|"*.hpp" PATTERN "xrst"|
EOF
edit_file CMakeLists.txt
#
cat << EOF > temp.sed
/{xrst_spell/! b one
: loop
N
/\\n}/! b loop
d
b end
#
: one
s|{xrst_begin CppAD}|&\\
{xrst_spell\\
   png\\
   github\\
   appveyor\\
   autodiff\\
   templated\\
   posix\\
}|
#
: end
EOF
edit_file doc.xrst
# -----------------------------------------------------------------------------
cat << EOF > temp.sed
/which run_omhelp.sh/! b end
N
N
N
s|which run_omhelp.sh|which xrst|
s|omhelp|xrst|
s/\n      echo_eval *run_omhelp.sh doc/\\
      sed -i xrst.toml -e "s|^default *=.*|default = 'doc.xrst'|"\\
      echo_eval xrst \\\\\\
         --local_toc \\\\\\
         --html_theme sphinx_rtd_theme \\\\\\
         --output_dir doc/
#
: end
EOF
edit_file bin/package.sh
# -----------------------------------------------------------------------------
file=xrst.toml
cp $file ~/trash
sed -n -e '1,/^latex_macro = \[/p'      $file > temp.1
sed -n -e '/\[project_dictionary\]/,$p' $file > temp.3
cat << EOF > temp.2
   '\\newcommand{\\W}[1]{ \\; #1 \\; }',
   '\\newcommand{\\R}[1]{ {\\rm #1} }',
   '\\newcommand{\\B}[1]{ {\\bf #1} }',
   '\\newcommand{\\D}[2]{ \\frac{\\partial #1}{\\partial #2} }',
   '\\newcommand{\\DD}[3]{ \\frac{\\partial^2 #1}{\\partial #2 \\partial #3} }',
   '\\newcommand{\\Dpow}[2]{ \\frac{\\partial^{#1}}{\\partial  {#2}^{#1}} }',
   '\\newcommand{\\dpow}[2]{ \\frac{ {\\rm d}^{#1}}{{\\rm d}\\, {#2}^{#1}} }',
]

EOF
cat temp.1 temp.2 temp.3 > $file
echo '--------------------------------------------------------------------'
if diff $file ~/trash
then
   echo "$file: no changes"
else
   echo "$file: changes above"
fi
# -----------------------------------------------------------------------------
echo 'fix_xrst.sh: OK'
exit 0
