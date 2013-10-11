#! /bin/python
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
import sys
import os
import re
# -----------------------------------------------------------------------------
if sys.argv[0] != 'bin/replace_html.py' :
	msg = 'bin/replace_html.py: must be executed from its parent directory'
	sys.exit(msg)
# 
usage = '''\nusage: replace_html.py file_path
where file_path is the path to the file with html text that is to be replaced. 

Each set of replacement text is defined in the file by
	<!-- define name -->source<!-- end name -->
where name is any unique name, with no spaces ' ' 
for the replacement text source and source can be any text. 

The destination, where the replacement text is placed, is specified by
	<!-- replace name -->desination<!-- end name -->
where name refers to a defined replacement text and destination
is the text that is replaced.

Note that this overwrites the original file and so you may want to first make
a backup copy.
'''
narg = len(sys.argv)
if narg != 2 :
	msg = '\nExpected 1 but found ' + str(narg-1) + ' command line arguments.'
	sys.exit(usage + msg)
file_path = sys.argv[1]
# -----------------------------------------------------------------------------
if not os.path.exists(file_path) :
	msg = 'bin/replace_html.py: cannot find the file ' + file_path
	sys.exit(msg)
f_in    = open(file_path, 'rb')
data_in = f_in.read()
f_in.close()
# -----------------------------------------------------------------------------
# create define: a dictionary with replacement text definitions
define    = {}
p_define  = re.compile('<!-- define ([^ ]*) -->')
p_end     = re.compile('<!-- end ([^ ]*) -->')
start     = 0
while start < len(data_in) :
	rest         = data_in[start : ]
	next_define  = p_define.search(rest)
	if next_define == None :
		start = len(data_in)
	else :
		name         = next_define.group(1)
		if name in define :
			msg  = 'bin/replace_html.py: file = ' + file_path
			msg += '\ncontains two defintions for name = ' + name
			sys.exit(msg)
		rest         = rest[ next_define.end() : ]
		#
		next_end     = p_end.search(rest)
		source       = rest [ 0 : next_end.start() ]
		define[name] = source
		start       += next_define.end() + next_end.end()
		if name != next_end.group(1) :
			msg  = 'bin/replace_html.py: file = ' + file_path
			msg += '\ndefine name = ' + name
			msg += ', end name = ' + next_end.group(1)
			sys.exit(msg)
# -----------------------------------------------------------------------------
# create data_out: a string with the replacements made
data_out  = ''
p_replace = re.compile('<!-- replace ([^ ]*) -->')
start     = 0
while start < len(data_in) :
	rest          = data_in[start : ]
	next_replace  = p_replace.search(rest)
	if next_replace == None :
		data_out += rest
		start     = len(data_in)
	else :
		name      = next_replace.group(1)
		if name not in define :
			msg  = 'bin/replace_html.py: file = ' + file_path
			msg += '\ncontains no defintions for name = ' + name
			sys.exit(msg)
		data_out    += rest[0 : next_replace.end() ]
		data_out    += define[name]
		#
		rest         = rest[ next_replace.end() : ]
		next_end     = p_end.search(rest)
		data_out    += rest[ next_end.start() : next_end.end() ]
		start       += next_replace.end() + next_end.end()
		if name != next_end.group(1) :
			msg  = 'bin/replace_html.py: file = ' + file_path
			msg += '\nreplace name = ' + name
			msg += ', end name = ' + next_end.group(1)
			sys.exit(msg)
# -----------------------------------------------------------------------------
f_out    = open(file_path, 'wb')
f_out.write(data_out)
f_out.close()
# -----------------------------------------------------------------------------
sys.exit(0)
