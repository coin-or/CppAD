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
'base_adolc.hpp.htm'
];
var list_current0 = [
'base_adolc.hpp.htm#Syntax',
'base_adolc.hpp.htm#Example',
'base_adolc.hpp.htm#Include Files',
'base_adolc.hpp.htm#CondExpOp',
'base_adolc.hpp.htm#CondExpRel',
'base_adolc.hpp.htm#EqualOpSeq',
'base_adolc.hpp.htm#Identical',
'base_adolc.hpp.htm#Integer',
'base_adolc.hpp.htm#azmul',
'base_adolc.hpp.htm#Ordered',
'base_adolc.hpp.htm#Unary Standard Math',
'base_adolc.hpp.htm#erfc',
'base_adolc.hpp.htm#sign',
'base_adolc.hpp.htm#abs',
'base_adolc.hpp.htm#pow',
'base_adolc.hpp.htm#numeric_limits',
'base_adolc.hpp.htm#to_string',
'base_adolc.hpp.htm#hash_code'
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
