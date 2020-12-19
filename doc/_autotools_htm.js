var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'appendix.htm',
'deprecated.htm',
'autotools.htm'
];
var list_current0 = [
'autotools.htm#Deprecated 2012-12-26',
'autotools.htm#Distribution Directory',
'autotools.htm#Build Directory',
'autotools.htm#Configure',
'autotools.htm#make',
'autotools.htm#make.Examples and Tests',
'autotools.htm#Profiling CppAD',
'autotools.htm#prefix_dir',
'autotools.htm#--with-Documentation',
'autotools.htm#--with-testvector',
'autotools.htm#--enable-msvc',
'autotools.htm#max_num_threads',
'autotools.htm#cxx_flags',
'autotools.htm#openmp_flags',
'autotools.htm#postfix_dir',
'autotools.htm#adolc_dir',
'autotools.htm#adolc_dir.Linux',
'autotools.htm#adolc_dir.Cygwin',
'autotools.htm#boost_dir',
'autotools.htm#eigen_dir',
'autotools.htm#fadbad_dir',
'autotools.htm#ipopt_dir',
'autotools.htm#sacado_dir',
'autotools.htm#tape_addr_type',
'autotools.htm#tape_id_type',
'autotools.htm#make install'
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
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
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
