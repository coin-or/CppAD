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
'atomic_three.htm',
'atomic_three_afun.htm'
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
'chkpoint_two.htm'
];
var list_down1 = [
'atomic_three_ctor.htm',
'atomic_three_afun.htm',
'atomic_three_for_type.htm',
'atomic_three_forward.htm',
'atomic_three_reverse.htm',
'atomic_three_jac_sparsity.htm',
'atomic_three_hes_sparsity.htm',
'atomic_three_rev_depend.htm'
];
var list_current0 = [
'atomic_three_afun.htm#Syntax',
'atomic_three_afun.htm#Prototype',
'atomic_three_afun.htm#Purpose',
'atomic_three_afun.htm#Base',
'atomic_three_afun.htm#ADVector',
'atomic_three_afun.htm#afun',
'atomic_three_afun.htm#ax',
'atomic_three_afun.htm#ay'
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
