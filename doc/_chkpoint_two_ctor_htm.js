var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'ad.htm',
'advalued.htm',
'atomic.htm',
'chkpoint_two.htm',
'chkpoint_two_ctor.htm'
];
var list_down3 = [
'arithmetic.htm',
'unary_standard_math.htm',
'binary_math.htm',
'condexp.htm',
'discrete.htm',
'numeric_limits.htm',
'atomic.htm'
];
var list_down2 = [
'atomic_three.htm',
'atomic_three_example.htm',
'chkpoint_one.htm',
'chkpoint_two.htm'
];
var list_down1 = [
'chkpoint_two_ctor.htm',
'chkpoint_two_chk_fun.htm',
'chkpoint_two_dynamic.htm',
'chkpoint_two_get_started.cpp.htm',
'chkpoint_two_compare.cpp.htm',
'chkpoint_two_base2ad.cpp.htm',
'chkpoint_two_dynamic.cpp.htm',
'chkpoint_two_ode.cpp.htm'
];
var list_current0 = [
'chkpoint_two_ctor.htm#Syntax',
'chkpoint_two_ctor.htm#Prototype',
'chkpoint_two_ctor.htm#Parallel',
'chkpoint_two_ctor.htm#Base',
'chkpoint_two_ctor.htm#fun',
'chkpoint_two_ctor.htm#name',
'chkpoint_two_ctor.htm#internal_bool',
'chkpoint_two_ctor.htm#use_hes_sparsity',
'chkpoint_two_ctor.htm#use_base2ad',
'chkpoint_two_ctor.htm#use_in_parallel',
'chkpoint_two_ctor.htm#chk_fun'
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
