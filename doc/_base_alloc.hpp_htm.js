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
