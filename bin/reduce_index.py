#! /bin/python3
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
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
# list of words that should be excluded from mindex comamnds
exclude_list = 'a an and as at by for in of or to'.split()
# -----------------------------------------------------------------------------
def split_words(string) :
	pattern = '(\s|,)'
	replace = ' '
	string  = re.sub(pattern, replace, string)
	return string.split()
# -----------------------------------------------------------------------------
if sys.argv[0] != 'bin/reduce_index.py' :
	msg = 'bin/reduce_index.py: must be executed from its parent directory'
	sys.exit(msg)
#
usage = 'usage: reduce_index.py file_name'
narg = len(sys.argv)
if narg != 2 :
	msg = '\n\tFound ' + str(narg-1) + ' command line arguments (expected 1).'
	sys.exit(usage + msg)
file_name  = sys.argv[1]
# -----------------------------------------------------------------------------
if not os.path.exists(file_name) :
	msg = 'bin/reduce_index.py: cannot find file_name = ' + file_name
	sys.exit(msg)
f_in      = open(file_name, 'r')
file_data = f_in.read()
f_in.close()
f_out     = open(file_name, 'w')
# -----------------------------------------------------------------------------
# some useful patterns
begin_pattern   = re.compile('[$]begin ([^$]*)[$][$]')
end_pattern     = re.compile('[$]end')
section_pattern = re.compile('[$](section)\s([^$]*)[$][$]')
index_pattern   = re.compile('[$][mc]{0,1}(index) ([^$]*)[$][$]')
cmd_pattern     = re.compile('[$]([a-z]*) ([^$]*)[$][$]')
# -----------------------------------------------------------------------------
def remove_index_cmd(string) :
	result     = str()
	rest       = string
	next_index = index_pattern.search(rest)
	while next_index != None :
		result    += rest[ : next_index.start() ]
		rest       = rest[ next_index.end() : ]
		if len(rest) > 0 :
			if rest[0] == '\n' :
				rest = rest[1 : ]
		next_index = index_pattern.search(rest)
	result += rest
	return result
#
# for each omhelp section in the file
file_rest  = file_data
while len(file_rest) > 0 :
	# start of this section
	next_begin = begin_pattern.search(file_rest)
	if next_begin == None :
		f_out.write( file_rest )
		file_rest = str()
	else :
		# end of this section
		next_end   = end_pattern.search(file_rest)
		if next_end == None :
			sys.exit( 'No $end after' + next_begin(0) )
		if next_end.end() < next_begin.end() :
			sys.exit( 'No $end after' + next_begin(0) )
		# data for this section
		section_data = file_rest [ : next_end.end() ]
		file_rest    = file_rest [ next_end.end() : ]
		#
		# get list of automatically generated index words for this section
		section_rest = section_data
		auto_list    = list()
		while len(section_rest) > 0 :
			next_cmd = cmd_pattern.search(section_rest)
			if next_cmd == None :
				section_rest = str()
			else :
				section_rest = section_rest[ next_cmd.end() : ]
				cmd          = next_cmd.group(1)
				if cmd in [ 'section', 'head', 'subhead' ] :
					for word in split_words( next_cmd.group(2) ) :
						auto_list.append( word.lower() )
		#
		# list of index words not in automatically generated list
		section_rest  = section_data
		index_list    = list()
		exclude_list += auto_list
		for word in auto_list :
			if word.endswith('s') :
				exclude_list.append( word[: -1] )
			else :
				exclude_list.append( word + 's' )
		while len(section_rest) > 0 :
			next_index = index_pattern.search(section_rest)
			if next_index == None :
				section_rest = str()
			else :
				section_rest = section_rest[ next_index.end() : ]
				for word in split_words( next_index.group(2) ) :
					word_lower = word.lower()
					if not ( word_lower in exclude_list ) :
						index_list.append(word)
						exclude_list.append(word_lower)
		# keep words that are not in start of other words
		keep_index = list()
		for word_1 in index_list :
			ok_1 = True
			for word_2 in index_list :
				if word_2.startswith(word_1 + '_') :
					ok_1 = False
				if word_2.endswith('_' + word_1) :
					ok_1 = False
			if ok_1 :
				keep_index.append(word_1)
		#
		# write out data to the end of section command
		section_cmd  = section_pattern.search(section_data)
		data = remove_index_cmd( section_data[ : section_cmd.end() ] )
		f_out.write( data )
		if len(keep_index) > 0 :
			index_cmd = '$mindex'
			for word in keep_index :
				index_cmd += ' ' + word
			index_cmd += '$$'
			f_out.write( '\n' + index_cmd )
		data = remove_index_cmd( section_data[ section_cmd.end() : ] )
		f_out.write( data )
# -----------------------------------------------------------------------------
f_out.close()
print('reduce_index.py OK: ' + file_name )
sys.exit(0)
