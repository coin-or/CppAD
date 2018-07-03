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
'forward.htm',
'forward_dir.htm'
];
var list_down3 = [
'install.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down2 = [
'record_adfun.htm',
'drivers.htm',
'forward.htm',
'reverse.htm',
'sparsity_pattern.htm',
'sparse_derivative.htm',
'optimize.htm',
'abs_normal.htm',
'funcheck.htm',
'check_for_nan.htm'
];
var list_down1 = [
'new_dynamic.htm',
'forward_zero.htm',
'forward_one.htm',
'forward_two.htm',
'forward_order.htm',
'forward_dir.htm',
'size_order.htm',
'compare_change.htm',
'capacity_order.htm',
'number_skip.htm'
];
var list_down0 = [
'forward_dir.cpp.htm'
];
var list_current0 = [
'forward_dir.htm#Syntax',
'forward_dir.htm#Purpose',
'forward_dir.htm#Reverse Mode',
'forward_dir.htm#Notation',
'forward_dir.htm#Notation.n',
'forward_dir.htm#Notation.m',
'forward_dir.htm#f',
'forward_dir.htm#q',
'forward_dir.htm#r',
'forward_dir.htm#xq',
'forward_dir.htm#Zero Order',
'forward_dir.htm#Non-Zero Lower Orders',
'forward_dir.htm#X(t)',
'forward_dir.htm#Y(t)',
'forward_dir.htm#yq',
'forward_dir.htm#Vector',
'forward_dir.htm#Example'
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
