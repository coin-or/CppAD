var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'utility.htm',
'nan.htm'
];
var list_current0 = [
'nan.htm#Syntax',
'nan.htm#Purpose',
'nan.htm#Include',
'nan.htm#Include.Macros',
'nan.htm#isnan',
'nan.htm#isnan.s',
'nan.htm#isnan.b',
'nan.htm#hasnan',
'nan.htm#hasnan.v',
'nan.htm#hasnan.b',
'nan.htm#nan(zero)',
'nan.htm#nan(zero).Deprecated 2015-10-04',
'nan.htm#nan(zero).Syntax',
'nan.htm#nan(zero).z',
'nan.htm#nan(zero).s',
'nan.htm#Scalar',
'nan.htm#Vector',
'nan.htm#Example'
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
