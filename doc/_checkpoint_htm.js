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
'checkpoint.htm'
];
var list_down3 = [
'ad_ctor.htm',
'ad_assign.htm',
'convert.htm',
'advalued.htm',
'boolvalued.htm',
'vecad.htm',
'base_require.htm'
];
var list_down2 = [
'arithmetic.htm',
'unary_standard_math.htm',
'binary_math.htm',
'condexp.htm',
'discrete.htm',
'numeric_limits.htm',
'atomic.htm'
];
var list_down1 = [
'checkpoint.htm',
'atomic_base.htm',
'atomic_example.htm'
];
var list_down0 = [
'checkpoint.cpp.htm',
'atomic_mul_level.cpp.htm',
'checkpoint_ode.cpp.htm',
'checkpoint_extended_ode.cpp.htm'
];
var list_current0 = [
'checkpoint.htm#Syntax',
'checkpoint.htm#See Also',
'checkpoint.htm#Purpose',
'checkpoint.htm#Purpose.Reduce Memory',
'checkpoint.htm#Purpose.Faster Recording',
'checkpoint.htm#Purpose.Repeating Forward',
'checkpoint.htm#Purpose.Restriction',
'checkpoint.htm#Purpose.Multiple Level AD',
'checkpoint.htm#Method',
'checkpoint.htm#constructor',
'checkpoint.htm#Base',
'checkpoint.htm#ADVector',
'checkpoint.htm#name',
'checkpoint.htm#ax',
'checkpoint.htm#ay',
'checkpoint.htm#sparsity',
'checkpoint.htm#optimize',
'checkpoint.htm#size_var',
'checkpoint.htm#option',
'checkpoint.htm#algo',
'checkpoint.htm#atom_fun',
'checkpoint.htm#clear',
'checkpoint.htm#clear.Restriction',
'checkpoint.htm#Example'
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
