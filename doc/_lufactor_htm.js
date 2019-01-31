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
'lufactor.htm'
];
var list_current0 = [
'lufactor.htm#Syntax',
'lufactor.htm#Description',
'lufactor.htm#Include',
'lufactor.htm#Matrix Storage',
'lufactor.htm#sign',
'lufactor.htm#ip',
'lufactor.htm#jp',
'lufactor.htm#LU',
'lufactor.htm#LU.A',
'lufactor.htm#LU.P',
'lufactor.htm#LU.L',
'lufactor.htm#LU.U',
'lufactor.htm#LU.Factor',
'lufactor.htm#LU.Determinant',
'lufactor.htm#SizeVector',
'lufactor.htm#FloatVector',
'lufactor.htm#Float',
'lufactor.htm#AbsGeq',
'lufactor.htm#Example',
'lufactor.htm#Source'
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
