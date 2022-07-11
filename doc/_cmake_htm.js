var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'install.htm',
'cmake.htm'
];
var list_current0 = [
'cmake.htm#The CMake Program',
'cmake.htm#Build Directory',
'cmake.htm#CMake Command',
'cmake.htm#CMake Command.Simple',
'cmake.htm#CMake Command.Warning',
'cmake.htm#CMake Command.Options',
'cmake.htm#CMake Command.msys2',
'cmake.htm#CMake Command.Visual Studio',
'cmake.htm#CMake Command.autotools',
'cmake.htm#make check',
'cmake.htm#cmake_verbose_makefile',
'cmake.htm#cmake_build_type',
'cmake.htm#cmake_needs_dot_slash',
'cmake.htm#generator',
'cmake.htm#cppad_prefix',
'cmake.htm#cppad_postfix',
'cmake.htm#cmake_install_includedirs',
'cmake.htm#cmake_install_libdirs',
'cmake.htm#cmake_install_libdirs.cppad_lib',
'cmake.htm#cmake_install_datadir',
'cmake.htm#cmake_install_docdir',
'cmake.htm#cmake_defined_ok',
'cmake.htm#include_adolc',
'cmake.htm#include_eigen',
'cmake.htm#include_ipopt',
'cmake.htm#include_cppadcg',
'cmake.htm#package_prefix',
'cmake.htm#cppad_cxx_flags',
'cmake.htm#cppad_cxx_flags.C++17',
'cmake.htm#cppad_cxx_flags.debug and release',
'cmake.htm#cppad_profile_flag',
'cmake.htm#cppad_testvector',
'cmake.htm#cppad_max_num_threads',
'cmake.htm#cppad_tape_id_type',
'cmake.htm#cppad_tape_id_type.cstdint',
'cmake.htm#cppad_tape_addr_type',
'cmake.htm#cppad_tape_addr_type.cstdint',
'cmake.htm#cppad_debug_which'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
