var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'adfun.htm',
'optimize.htm'
];
var list_current0 = [
'optimize.htm#Syntax',
'optimize.htm#Purpose',
'optimize.htm#f',
'optimize.htm#options',
'optimize.htm#options.no_conditional_skip',
'optimize.htm#options.no_compare_op',
'optimize.htm#options.no_print_for_op',
'optimize.htm#Re-Optimize',
'optimize.htm#Efficiency',
'optimize.htm#Taylor Coefficients',
'optimize.htm#Speed Testing',
'optimize.htm#Atomic Functions',
'optimize.htm#Atomic Functions.rev_sparse_jac',
'optimize.htm#Atomic Functions.nan',
'optimize.htm#Checking Optimization',
'optimize.htm#Examples'
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
