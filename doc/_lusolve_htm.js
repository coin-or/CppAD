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
'ludetandsolve.htm',
'lusolve.htm'
];
var list_current0 = [
'lusolve.htm#Syntax',
'lusolve.htm#Description',
'lusolve.htm#Include',
'lusolve.htm#Factor and Invert',
'lusolve.htm#Matrix Storage',
'lusolve.htm#signdet',
'lusolve.htm#n',
'lusolve.htm#m',
'lusolve.htm#A',
'lusolve.htm#B',
'lusolve.htm#X',
'lusolve.htm#logdet',
'lusolve.htm#Float',
'lusolve.htm#FloatVector',
'lusolve.htm#LeqZero',
'lusolve.htm#AbsGeq',
'lusolve.htm#Example',
'lusolve.htm#Source'
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
