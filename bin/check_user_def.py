#! /usr/bin/env python3
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-24 Bradley M. Bell
# ----------------------------------------------------------------------------
import re
import sys
import subprocess
#
# p_symbol_heading
p_symbol_heading = re.compile( r'\n(CPPAD[A-Z_]*)\n([^\n]*)\n' )
#
# p_symbol_undef
p_symbol_undef = re.compile( r'\n# undef (CPPAD[A-Z_]*)\n' )
#
def main() :
   #
   if sys.argv[0] != 'bin/check_user_def.py' :
      sys.exit(
         'bin/check_user_def.py must execute from its parent directory'
      )
   #
   # exclude_list
   exclude_list = [
      'include/cppad/configure.hpp.in',
      'include/cppad/core/vec_ad/vec_ad.hpp',
      'configure',
]

   #
   # file_list
   command = [ 'git', 'ls-files' ]
   result = subprocess.run(
         command                   ,
         stdout = subprocess.PIPE  ,
         stderr = subprocess.PIPE  ,
         text   = True             ,
   )
   if result.returncode != 0 :
      msg  = 'Error: ' + ' '.join(command) + '\n'
      msg += result.stderr
      sys.exit(msg)
   file_list = result.stdout.strip().split('\n')
   #
   # file_list
   file_tmp = list()
   for file in file_list :
      exclude = file in exclude_list
      if file.startswith('include/cppad/local/') :
         exclude = exclude or file.endswith('/hpp')
      if not exclude :
         file_tmp.append(file)
   file_list = file_tmp
   #
   # preprocessor_data
   file_obj          = open('xrst/preprocessor.xrst', 'r')
   preprocessor_data = file_obj.read()
   file_obj.close()
   #
   # undef_list
   undef_list = list()
   #
   # m_symbol_undef
   m_symbol_undef = p_symbol_undef.search( preprocessor_data )
   while m_symbol_undef != None :
      undef_list.append( m_symbol_undef.group(1) )
      #
      # m_symbol_undef
      start           = m_symbol_undef.end() - 1
      m_symbol_undef = p_symbol_undef.search( preprocessor_data , start)
   #
   # file
   for file in file_tmp :
      #
      # file_data
      file_obj  = open(file, 'r')
      try :
         file_data = file_obj.read()
      except :
         file_data = ''
      file_obj.close()
      #
      # m_symbol_heading
      m_symbol_heading = p_symbol_heading.search( file_data )
      while m_symbol_heading != None :
         #
         # error
         symbol    = m_symbol_heading.group(1)
         underline = m_symbol_heading.group(2)
         error     = False
         if len(symbol) == len(underline) :
            if underline == len(symbol) * underline[0] :
               if symbol not in undef_list :
                  error = True
         if error :
            msg  = f'{file}: {symbol} appears in this file\n'
            msg += 'but it is supposed to be in the user API.'
            sys.exit(msg)
         #
         # m_symbol_heading
         start            = m_symbol_heading.end()
         m_symbol_heading = p_symbol_heading.search( file_data , start)
#
main()
print('check_user_def.py: OK')
