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
'base_require.htm',
'base_example.htm',
'base_complex.hpp.htm'
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
'base_member.htm',
'base_cond_exp.htm',
'base_identical.htm',
'base_ordered.htm',
'base_std_math.htm',
'base_limits.htm',
'base_to_string.htm',
'base_hash.htm',
'base_example.htm'
];
var list_down1 = [
'base_alloc.hpp.htm',
'base_require.cpp.htm',
'base_adolc.hpp.htm',
'base_float.hpp.htm',
'base_double.hpp.htm',
'base_complex.hpp.htm'
];
var list_down0 = [
'complex_poly.cpp.htm'
];
var list_current0 = [
'base_complex.hpp.htm#Example',
'base_complex.hpp.htm#Include Order',
'base_complex.hpp.htm#CondExpOp',
'base_complex.hpp.htm#CondExpRel',
'base_complex.hpp.htm#EqualOpSeq',
'base_complex.hpp.htm#Identical',
'base_complex.hpp.htm#Ordered',
'base_complex.hpp.htm#Integer',
'base_complex.hpp.htm#azmul',
'base_complex.hpp.htm#isnan',
'base_complex.hpp.htm#Valid Unary Math',
'base_complex.hpp.htm#Invalid Unary Math',
'base_complex.hpp.htm#pow',
'base_complex.hpp.htm#numeric_limits',
'base_complex.hpp.htm#to_string'
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
