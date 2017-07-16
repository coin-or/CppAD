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
'base_alloc.hpp.htm'
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
var list_current0 = [
'base_alloc.hpp.htm#Purpose',
'base_alloc.hpp.htm#Include File',
'base_alloc.hpp.htm#Compound Assignment Macro',
'base_alloc.hpp.htm#Binary Operator Macro',
'base_alloc.hpp.htm#Boolean Operator Macro',
'base_alloc.hpp.htm#Class Definition',
'base_alloc.hpp.htm#CondExpOp',
'base_alloc.hpp.htm#CondExpRel',
'base_alloc.hpp.htm#EqualOpSeq',
'base_alloc.hpp.htm#Identical',
'base_alloc.hpp.htm#Output Operator',
'base_alloc.hpp.htm#Integer',
'base_alloc.hpp.htm#azmul',
'base_alloc.hpp.htm#Ordered',
'base_alloc.hpp.htm#Unary Standard Math',
'base_alloc.hpp.htm#erf, asinh, acosh, atanh, expm1, log1p',
'base_alloc.hpp.htm#sign',
'base_alloc.hpp.htm#pow',
'base_alloc.hpp.htm#numeric_limits',
'base_alloc.hpp.htm#to_string',
'base_alloc.hpp.htm#hash_code'
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
