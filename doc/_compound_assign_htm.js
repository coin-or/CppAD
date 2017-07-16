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
'arithmetic.htm',
'compound_assign.htm'
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
'unaryplus.htm',
'unaryminus.htm',
'ad_binary.htm',
'compound_assign.htm'
];
var list_down0 = [
'addeq.cpp.htm',
'sub_eq.cpp.htm',
'mul_eq.cpp.htm',
'div_eq.cpp.htm'
];
var list_current0 = [
'compound_assign.htm#Syntax',
'compound_assign.htm#Purpose',
'compound_assign.htm#Op',
'compound_assign.htm#Base',
'compound_assign.htm#x',
'compound_assign.htm#y',
'compound_assign.htm#Result',
'compound_assign.htm#Operation Sequence',
'compound_assign.htm#Example',
'compound_assign.htm#Derivative',
'compound_assign.htm#Derivative.Addition',
'compound_assign.htm#Derivative.Subtraction',
'compound_assign.htm#Derivative.Multiplication',
'compound_assign.htm#Derivative.Division'
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
