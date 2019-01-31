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
'rombergmul.htm'
];
var list_current0 = [
'rombergmul.htm#Syntax',
'rombergmul.htm#Description',
'rombergmul.htm#Include',
'rombergmul.htm#m',
'rombergmul.htm#r',
'rombergmul.htm#F',
'rombergmul.htm#a',
'rombergmul.htm#b',
'rombergmul.htm#n',
'rombergmul.htm#p',
'rombergmul.htm#e',
'rombergmul.htm#Float',
'rombergmul.htm#FloatVector',
'rombergmul.htm#Example',
'rombergmul.htm#Source Code'
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
