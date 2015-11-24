#! /bin/python3
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
# dictionary of replacements
#
import collections
replace_dict = collections.OrderedDict()
replace_dict['projectName'] = 'CppAD'
replace_dict['projectRepo'] = 'CppAD'
#
replace_dict['projectDescription'] = '''
Given a C++ algorithm that computes function values,
CppAD generates an algorithm that computes corresponding derivative values.
'''
#
replace_dict['projectShortDescription'] ='''
A tool for differentiation of C++ functions.
'''
#
replace_dict['projectManager'] ='Brad Bell, http://www.seanet.com/~bradbell'
replace_dict['projectHomePage'] = 'http://www.coin-or.org/CppAD'
#
replace_dict['projectLicense'] = '''
Eclipse Public License 1.0, or GNU General Public License 3.0
'''
replace_dict['projectLicenseURL'] = '''
http://www.opensource.org/licenses/alphabetica
'''
replace_dict['coinLinkedProjects'] = ''
replace_dict['projectLanguage'] = 'C++'
replace_dict['activityStatus'] = 'Active'
replace_dict['maturityLevel'] = '4'
replace_dict['stableVersionNumber'] = '20150000'
replace_dict['releaseNumber'] = '20150000.8'
replace_dict['projectCategories'] = 'Optimization utility'
replace_dict['documentation'] = 'http://www.coin-or.org/CppAD/Doc'
replace_dict['sourceCodeDownload'] = '''
http://www.coin-or.org/download/source/CppAD
'''
replace_dict['mailingList'] = '''
http://list.coin-or.org/mailman/listinfo/cppad
'''
# ----------------------------------------------------------------------------
# <otherLinkedPackages>
#
other_package = collections.OrderedDict( [
('ADOL-C',             'http://www.coin-or.org/projects/ADOL-C.xml'),
('Boost uBlas vector', 'http://www.boost.org/doc/libs'),
('Boost Thread',       'http://www.boost.org/doc/libs'),
('ColPack',            'http://cscapes.cs.purdue.edu/dox/ColPack/html'),
('Eigen',              'http://eigen.tuxfamily.org'),
('FADBAD',             'http://www.fadbad.com/fadbad.htm'),
('Ipopt',              'http://www.coin-or.org/projects/Ipopt.xml'),
('OpenMP',             'http://openmp.org/wp'),
('Pthread',
	'http,//pubs.opengroup.org/onlinepubs/009695399/basedefs/pthread.h.html'),
('Sacado',             'http://trilinos.org/packages/sacado')
] )
text='\n'
for name in other_package :
	url  = other_package[name]
	text += '     <otherPackage>\n'
	text += '          <packageName>' + name + '</packageName>\n'
	text += '          <packageURL>' + url + '</packageURL>\n'
	text += '          <requiredOrOptional>Optional</requiredOrOptional>\n'
	text += '     </otherPackage>\n'
replace_dict['otherLinkedPackages'] = text
# -----------------------------------------------------------------------------
# <testedPlatforms>
tested_platform = collections.OrderedDict( [
('Linux',              'gcc'),
('Cygwin',             'gcc'),
('Microsoft Windows',  'MSVC++')
] )
text='\n'
for system in tested_platform :
	compiler = tested_platform[system]
	text += '     <platform>\n'
	text += '          <operatingSystem>' + system + '</operatingSystem>\n'
	text += '          <compiler>' + compiler + '</compiler>\n'
	text += '     </platform>\n'
replace_dict['testedPlatforms'] = text
# -----------------------------------------------------------------------------
import sys
import re
import os
import subprocess
# -----------------------------------------------------------------------------
# command line arguments
usage = '\tbin/proj_desc.py\n'
narg  = len(sys.argv)
if sys.argv[0] != 'bin/proj_desc.py' :
	msg = 'bin/proj_desc.py must be executed from its parent directory'
	sys.exit(usage + msg)
if narg != 1 :
	msg = 'expected 0 but found ' + str(narg-1) + ' command line arguments'
	sys.exit(usage + msg)
# -----------------------------------------------------------------------------
def system_cmd(cmd) :
	try :
		output = subprocess.check_output(
			cmd,
			stderr=subprocess.STDOUT,
			shell=True
		)
	except subprocess.CalledProcessError as info :
		msg  = str( info.output , 'ascii' )
		msg += '\nbin/proj_desc.py exiting because command above failed'
		sys.exit(msg)
	return str( output, 'ascii' )
# -----------------------------------------------------------------------------
# update cppad_conf_dir
#
cppad_conf_dir = os.environ['HOME'] + '/cppad.svn/conf'
if os.path.exists(cppad_conf_dir ) :
	cmd = 'svn update ' + cppad_conf_dir
	print( cmd )
else :
	cmd = 'svn checkout https://projects.coin-or.org/svn/CppAD/conf '
	cmd += cppad_conf_dir
	print( cmd )
# -----------------------------------------------------------------------------
# get the current verison of the file
#
file_name = cppad_conf_dir + '/projDesc.xml'
file_ptr  = open(file_name, 'r')
file_data = file_ptr.read()
file_ptr.close()
# -----------------------------------------------------------------------------
# Do the replacemnets
#
# fix stableVersionNumber and releaseNumber (remove enclosing comment block)
pattern   = '<!--\s*<stableVersionNumber>'
replace   = '<stableVersionNumber>'
file_data = re.sub(pattern, replace, file_data)
pattern   = '</releaseNumber>\s*-->'
replace   = '</releaseNumber>'
file_data = re.sub(pattern, replace, file_data)
#
for key in replace_dict :
	value     = replace_dict[key]
	start     = '<' + key + '>'
	end       = '</' + key + '>'
	pattern   = '[ \t]*' + start + '[a-zA-Z\s]*' + end
	replace   = start + value + end
	file_data = re.sub(pattern, replace, file_data)
#
# projectCategories is a special case
key       = 'projectCategories'
value     = replace_dict[key]
start     = '<' + key + '>'
end       = '</' + key + '>'
pattern   = '[ \t]*' + start + '[ \t]*'
file_data = re.sub(pattern, replace, file_data)
#
pattern   = '[ \t]*' + end + '[ \t]*'
replace   = start + '\n     <category>\n          '
replace  += value + '\n     </category>\n' + end
file_data = re.sub(pattern, replace, file_data)
# -----------------------------------------------------------------------------
# replace input file
file_name = cppad_conf_dir + '/projDesc.xml'
file_ptr  = open(file_name, 'w')
file_ptr.write(file_data)
file_ptr.close()
# -----------------------------------------------------------------------------
msg  = 'check results in\n\t' + file_name + '\nand then execute'
msg += '\n\tsvn commit -m "see bin/proj_desc.py" \\\n\t' + file_name
print(msg)

