#! /bin/python
# $Id
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
from __future__ import print_function
# -----------------------------------------------------------------------------
# list of svn commands to execute in the svn directory before make changes
# indicated by git directory; some example commands are included below
svn_commands = [
	# 'svn mkdir  cppad/utility',
	# 'svn move cppad/*.hpp                    cppad/utility',
	# 'svn move cppad/utility/cppad.hpp        cppad/cppad.hpp',
	# 'svn move cppad/utility/base_require.hpp cppad/base_require.hpp',
	# 'svn move omh/library.omh                omh/utility.omh'
]
# -----------------------------------------------------------------------------
# imports
import sys
import os
import re
import subprocess
import pdb
# -----------------------------------------------------------------------------
# command line arguments
usage = '\tbin/push_git2svn.py svn_branch_path\n'
narg  = len(sys.argv)
if sys.argv[0] != 'bin/push_git2svn.py' :
	msg = 'bin/push_git2svn.py must be executed from its parent directory'
	sys.exit(usage + msg)
if narg != 2 :
	msg = 'expected 1 but found ' + str(narg-1) + ' command line arguments'
	sys.exit(usage + msg)
svn_branch_path = sys.argv[1]
if svn_branch_path == 'master' :
	msg = 'trunk is the svn_branch_path for the master branch'
	sys.exit(usage + msg)
# -----------------------------------------------------------------------------
# some settings
svn_repository = 'https://projects.coin-or.org/svn/CppAD'
git_repository = 'https://github.com/coin-or/CppAD'
work_directory = 'build/work'
# -----------------------------------------------------------------------------
# some simple functions
def pause(question, choice_list) :
	response=''
	while not response in choice_list :
		print(question, end='')
		response = sys.stdin.readline()
		if response.endswith('\n') :
			response = response[:-1]
	return response
#
def system(cmd) :
	try :
		output = subprocess.check_output(
			cmd,
			stderr=subprocess.STDOUT,
			shell=True
		)
	except subprocess.CalledProcessError as info :
		msg  = info.output
		msg += '\nbin/push_git2svn.py exiting because command above failed'
		sys.exit(cmd + '\n\n' + msg)
	return output
def print_system(cmd) :
	print(cmd)
	try :
		output = subprocess.check_output(
			cmd,
			stderr=subprocess.STDOUT,
			shell=True
		)
	except subprocess.CalledProcessError as info :
		msg  = info.output
		msg += '\nbin/push_git2svn.py exiting because command above failed'
		sys.exit(msg)
	return output

id_pattern    = re.compile(r'^.*\$Id.*$',      re.MULTILINE)
white_pattern = re.compile(r'[ \t]+$',         re.MULTILINE)
date_pattern  = re.compile(r'2003-[0-9][0-9]', re.MULTILINE)
def ignore_data(data) :
	data = re.sub(id_pattern,    '', data)
	data = re.sub(white_pattern, '', data)
	data = re.sub(date_pattern,  '', data)
	return data
# -----------------------------------------------------------------------------
# determine git_branch_name
if svn_branch_path == 'trunk' :
	git_branch_name = 'master'
	git_branch_path = svn_branch_path
elif svn_branch_path.startswith('branches/') :
	git_branch_name = svn_branch_path[len('branches/'):]
	git_branch_path = svn_branch_path
else :
	git_branch_name = svn_branch_path
	git_branch_path = 'branches/' + svn_branch_path
# -----------------------------------------------------------------------------
# hash code for the git branch
cmd = 'git show-ref origin/' + git_branch_name
git_hash_code = system(cmd)
pattern       = ' refs/remotes/origin/' + git_branch_name
git_hash_code = git_hash_code.replace(pattern, '')
# -----------------------------------------------------------------------------
# make sure work directory exists
if not os.path.isdir(work_directory) :
	os.makedirs(work_directory)
# -----------------------------------------------------------------------------
# checkout svn version of directory
svn_directory = work_directory + '/svn'
if os.path.isdir(svn_directory) :
	question    = 'Use existing svn directory:\n\t'
	question   +=  svn_directory + '\n'
	question   += 'or remove it and check out a new copy ? [use/new] '
	choice_list = [ 'use' , 'new' ]
	choice      = pause(question, choice_list)
	if choice == 'new' :
		cmd         = 'rm -r ' + svn_directory
		print_system(cmd)
else :
	choice      = 'new'
if choice == 'use' :
	cmd = 'svn revert --recursive ' + svn_directory
	print_system(cmd)
	cmd = 'svn update ' + svn_directory
	print_system(cmd)
	cmd        = 'svn status ' + svn_directory
	svn_status = system(cmd)
	svn_status = svn_status.split('\n')
	for entry in svn_status :
		if entry.startswith('?       ') :
			file_name = entry[8:]
			cmd = 'rm ' + file_name
			system(cmd)
else :
	cmd  = 'svn checkout '
	cmd +=  svn_repository + '/' + svn_branch_path + ' ' + svn_directory
	print_system(cmd)
# ----------------------------------------------------------------------------
tmp = os.getcwd()
os.chdir( svn_directory )
for cmd in svn_commands :
	assert cmd.startswith('svn')
	print_system(cmd)
os.chdir( tmp )
# ----------------------------------------------------------------------------
# git hash code corresponding to verison in svn directory
cmd           = 'svn info ' + svn_directory
svn_info      = system(cmd)
rev_pattern   = re.compile('Last Changed Rev: *([0-9]+)')
match         = re.search(rev_pattern, svn_info)
svn_revision  = match.group(1)
cmd           = 'svn log -r ' + svn_revision + ' ' + svn_directory
svn_log       = system(cmd)
hash_pattern  = re.compile('\nend   hash code: *([0-9a-f]+)')
match         = re.search(hash_pattern, svn_log)
if match :
	svn_hash_code = match.group(1)
else :
	svn_hash_code = None
# -----------------------------------------------------------------------------
# export the git verison of the directory
git_directory = work_directory + '/git'
if os.path.isdir(git_directory) :
	cmd = 'rm -r ' + git_directory
	print_system(cmd)
cmd  = 'svn export '
cmd +=  git_repository + '/' + git_branch_path + ' ' + git_directory
print_system(cmd)
# -----------------------------------------------------------------------------
# list of files for the svn and git directories
svn_pattern = re.compile(svn_directory + '/')
svn_file_list = []
svn_dir_list  = []
for directory, dir_list, file_list in os.walk(svn_directory) :
	ok = ( directory.find('/.svn/') == -1 )
	ok = ok and ( not directory.endswith('/.svn') )
	if ok :
		if directory != svn_directory :
			local_name = re.sub(svn_pattern, '', directory)
			svn_dir_list.append(local_name)
		for name in file_list :
			local_name = directory + '/' + name
			local_name = re.sub(svn_pattern, '', local_name)
			svn_file_list.append( local_name )
#
git_pattern = re.compile(git_directory + '/')
git_file_list = []
git_dir_list  = []
for directory, dir_list, file_list in os.walk(git_directory) :
	index =  directory.find('/.svn/')
	assert index == -1
	if directory != git_directory :
		local_name = re.sub(git_pattern, '', directory)
		git_dir_list.append(local_name)
	for name in file_list :
		local_name = directory + '/' + name
		local_name = re.sub(git_pattern, '', local_name)
		git_file_list.append( local_name )
# -----------------------------------------------------------------------------
# list of files that have been created and deleted
created_file_list=[]
for name in git_file_list :
	if not name in svn_file_list :
		created_file_list.append(name)
#
deleted_file_list=[]
for name in svn_file_list :
	if not name in git_file_list :
		deleted_file_list.append(name)
# -----------------------------------------------------------------------------
# list of directories that have been created and deleted
created_dir_list=[]
for name in git_dir_list :
	if not name in svn_dir_list :
		created_dir_list.append(name)
#
deleted_dir_list=[]
for name in svn_dir_list :
	if not name in git_dir_list :
		deleted_dir_list.append(name)
# -----------------------------------------------------------------------------
# automated svn commands
#
for git_dir in created_dir_list :
	cmd  = 'svn mkdir ' + svn_directory + '/' + git_dir
	print_system(cmd)
#
for git_file in created_file_list :
	git_f     = open(git_directory + '/' + git_file, 'rb')
	git_data  = git_f.read()
	git_f.close()
	git_data  = ignore_data(git_data)
	#
	found = False
	for svn_file in deleted_file_list :
		svn_f    = open(svn_directory + '/' + svn_file, 'rb')
		svn_data = svn_f.read()
		svn_f.close()
		svn_data = ignore_data(svn_data)
		#
		if svn_data == git_data :
			assert not found
			cmd  = 'svn copy ' + svn_directory + '/' + svn_file + ' \\\n\t'
			cmd += svn_directory + '/' + git_file
			print_system(cmd)
			cmd  = 'cp ' + git_directory + '/' + git_file + ' \\\n\t'
			cmd += svn_directory + '/' + git_file
			system(cmd)
			found = True
	if not found :
			cmd  = 'cp ' + git_directory + '/' + git_file + ' \\\n\t'
			cmd += svn_directory + '/' + git_file
			system(cmd)
			cmd  = 'svn add ' + svn_directory + '/' + git_file
			print_system(cmd)
#
for svn_file in deleted_file_list :
	svn_file_path = svn_directory + '/' + svn_file
	if os.path.isfile(svn_file_path) :
		cmd = 'svn delete --force ' + svn_file_path
		print_system(cmd)
#
for git_file in git_file_list :
	do_cp = True
	do_cp = do_cp and git_file not in created_file_list
	if git_file in svn_file_list :
		git_f     = open(git_directory + '/' + git_file, 'rb')
		git_data  = git_f.read()
		git_f.close()
		git_data  = ignore_data(git_data)
		#
		svn_f    = open(svn_directory + '/' + git_file, 'rb')
		svn_data = svn_f.read()
		svn_f.close()
		svn_data = ignore_data(svn_data)
		#
		do_cp = do_cp and git_data != svn_data
	if do_cp :
		cmd  = 'cp ' + git_directory + '/' + git_file + ' \\\n\t'
		cmd += svn_directory + '/' + git_file
		system(cmd)
#
for svn_dir in deleted_dir_list :
	cmd  = 'svn rm ' + svn_directory + '/' + svn_dir
	print_system(cmd)
# -----------------------------------------------------------------------------
data  = 'merge to branch: ' + svn_branch_path + '\n'
data += 'from repository: ' + git_repository + '\n'
if svn_hash_code != None :
	data += 'start hash code: ' + svn_hash_code + '\n'
else :
	data += 'start hash code: missing\n'
data   += 'end   hash code: ' + git_hash_code + '\n\n'
if svn_hash_code != None :
	sed_cmd = "sed -e '/" + svn_hash_code + "/,$d'"
	cmd     = 'git log origin/' + git_branch_name + ' | ' + sed_cmd
	output = system(cmd)
	data  += output
log_f  = open( svn_directory + '/push_git2svn.log' , 'wb')
log_f.write(data)
log_f.close()
#
msg  = '\nChange into svn directory with the command\n\t'
msg += 'cd ' + svn_directory + '\n'
msg += 'If these changes are OK, execute the command:\n\t'
msg += 'svn commit --file push_git2svn.log\n'
if svn_hash_code != None :
	msg += 'You should inspect and possibly edit push_git2svn.log'
else :
	msg += 'The start hash code could not be automatically determined.\n'
	msg += 'You should edit push_git2svn.log to describe the changes.'
print(msg)
