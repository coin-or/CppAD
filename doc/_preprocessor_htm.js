var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'preprocessor.htm'
];
var list_current0 = [
'preprocessor.htm#Rule',
'preprocessor.htm#NOMINMAX, windows.h',
'preprocessor.htm#Documented Here',
'preprocessor.htm#Documented Here.CPPAD_DEBUG_AND_RELEASE',
'preprocessor.htm#CPPAD_NULL',
'preprocessor.htm#CPPAD_NULL.CPPAD_PACKAGE_STRING',
'preprocessor.htm#CPPAD_USE_CPLUSPLUS_2011',
'preprocessor.htm#CPPAD_USE_CPLUSPLUS_2017',
'preprocessor.htm#Documented Elsewhere',
'preprocessor.htm#Documented Elsewhere.Deprecated'
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
