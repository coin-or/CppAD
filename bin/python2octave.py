#! /usr/bin/python3
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# imports
import sys
import os
import re
# -----------------------------------------------------------------------------
# command line arguments
usage  = '\tbin/python2octave.py name\n'
usage += 'uses the file swig/python/name.py to create the file swig/name.m.\n'
usage += 'This assumes a very simple structure used for test files.\n'
narg  = len(sys.argv)
if sys.argv[0] != 'bin/python2octave.py' :
	msg = 'bin/python2octave.py must be executed in its parent directory.'
	sys.exit(usage + msg)
if narg != 2 :
	msg = 'Expected 1 but found ' + str(narg-1) + 'command line arguments.'
	sys.exit(usage + msg)
name    = sys.argv[1]
name_in = 'swig/python/' + name + '.py'
if not os.path.exists(name_in) :
	msg = 'Cannot find the file ' + name_in + '.'
	sys.exit(usage + msg)
name_out = 'swig/octave/' + name + '.m'
# -----------------------------------------------------------------------------
# read data
file_in  = open(name_in, 'rb')
data_in  = file_in.read()
file_in.close()
data_out = data_in
# -----------------------------------------------------------------------------
# simple replacements
simple_replace = {
	b'#':b'%',
	b'True':b'true',
	b'False':b'false',
	b'bool\\(':b'logical(',
	b' and ':b' && '
}
for key in simple_replace :
	pattern  = key
	pattern  = re.compile(pattern)
	replace  = simple_replace[key]
	(data_out, n_sub) = re.subn(pattern, replace, data_out)
# -----------------------------------------------------------------------------
# semi-colon at end of lines
pattern  = b'$'
pattern  = re.compile(pattern, re.MULTILINE)
replace  = b';'
(data_out, n_sub) = re.subn(pattern, replace, data_out)
#
# not in comment lines
pattern = b'(%[^\\n]*);'
replace = b'\\1'
(data_out, n_sub) = re.subn(pattern, replace, data_out)
#
# not in empty lines
pattern = b'^;$'
pattern = re.compile(pattern, re.MULTILINE)
replace = b''
(data_out, n_sub) = re.subn(pattern, replace, data_out)
# -----------------------------------------------------------------------------
# function definition
pattern  = b'\ndef (\\w+) *\(\) *: *;\n'
pattern  = re.compile(pattern)
data_tmp = b''
match    = re.search(pattern, data_out)
msg      = 'Expected a single function function with no arguments.'
if match == None :
	sys.exit(usage + msg)
start     = match.start()
end       = match.end()
name      = match.group(1)
data_tmp += data_out[0 : start]
data_tmp += b'\nfunction ok = ' + name + b'()\n'
data_tmp += data_out[end :]
data_tmp += b'\nend'
data_out  = data_tmp
# -----------------------------------------------------------------------------
# imports
pattern  = b'^(\\t*)import  *(\\w+) *;$'
pattern  = re.compile(pattern, re.MULTILINE)
replace  = b'\\1\\2'
(data_out, n_sub) = re.subn(pattern, replace, data_out)
# -----------------------------------------------------------------------------
# prints
pattern  = b"^(\\t*)print *\\( *'([^']*)' *\\) *;$"
pattern  = re.compile(pattern, re.MULTILINE)
replace  = b"\\1printf('\\2\\\\n')"
(data_out, n_sub) = re.subn(pattern, replace, data_out)
# -----------------------------------------------------------------------------
# return
pattern  = b"\\n(\\t*)return "
pattern  = re.compile(pattern)
replace  = b'\n\\1ok = '
(data_out, n_sub) = re.subn(pattern, replace, data_out)
# -----------------------------------------------------------------------------
# vector indexing
pattern  = b'\\[([^\\]]+)\\]'
pattern  = re.compile(pattern)
replace  = b'(\\1)'
(data_out, n_sub) = re.subn(pattern, replace, data_out)
# -----------------------------------------------------------------------------
# for statements
pattern  = b'\\n(\\t*)for +(\\w+) +in +range\\( *(.*)\\) *: *;\\n'
pattern  = re.compile(pattern)
match    = re.search(pattern, data_out)
data_tmp = b''
index    = 0
match    = re.search(pattern, data_out)
while match != None :
	start     = match.start() + index
	end       = match.end() + index
	tabs      = match.group(1)
	limit     = match.group(3).replace(b' ', b'')
	data_tmp += data_out[index : start]
	data_tmp += b'\n' + tabs + b'for ' + match.group(2)
	data_tmp += b' = [ 0 : (' + limit + b'-1) ]\n'
	index     = end
	pattern2  = b'\\n' + len(tabs) * b'\\t' + b'[^\\t]'
	pattern2  = re.compile(pattern2)
	match     = re.search(pattern2, data_out[index :] )
	if match == None :
		msg   = 'Expected an end to for loop before end of file.'
	start     = match.start() + index
	data_tmp += data_out[index : start]
	data_tmp += b'\n' + tabs + b'end'
	index     = start
	match     = re.search(pattern, data_out[index :] )
data_tmp += data_out[index :]
data_out  = data_tmp
# -----------------------------------------------------------------------------
# if statements
pattern  = b'\\n(\\t*)if +(.*) *: *;\\n'
pattern  = re.compile(pattern)
match    = re.search(pattern, data_out)
data_tmp = b''
index    = 0
match    = re.search(pattern, data_out)
while match != None :
	start     = match.start() + index
	end       = match.end() + index
	tabs      = match.group(1)
	data_tmp += data_out[index : start]
	data_tmp += b'\n' + tabs + b'if ' + match.group(2) + b'\n'
	index     = end
	pattern2  = b'\\n' + len(tabs) * b'\\t' + b'else *: *;\\n'
	pattern2  = re.compile(pattern2)
	match     = re.search(pattern2, data_out[index :] )
	if match == None :
		msg   = 'Expected an else after if block.'
		sys.exit(usage + msg)
	start     = match.start() + index
	end       = match.end() + index
	data_tmp += data_out[index : start]
	data_tmp += b'\n' + tabs + b'else' + b'\n'
	index     = end
	pattern2  = b'\\n' + len(tabs) * b'\\t' + b'[^\\t]'
	pattern2  = re.compile(pattern2)
	match     = re.search(pattern2, data_out[index :] )
	if match == None :
		msg   = 'Expected an end to else block before end of file.'
		sys.exit(usage + msg)
	start     = match.start() + index
	data_tmp += data_out[index : start]
	data_tmp += b'\n' + tabs + b'end'
	index     = start
	match     = re.search(pattern, data_out[index :] )
data_tmp += data_out[index :]
data_out  = data_tmp
# -----------------------------------------------------------------------------
# remove spaces at end of line
pattern  = b' +$'
pattern  = re.compile(pattern, re.MULTILINE)
replace  = b''
(data_out, n_sub) = re.subn(pattern, replace, data_out)
# -----------------------------------------------------------------------------
file_out = open(name_out, 'wb')
file_out.write(data_out)
file_out.close()
# -----------------------------------------------------------------------------
print('python2octave.py: OK')
sys.exit(0)






