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
'simplevector.htm'
];
var list_current0 = [
'simplevector.htm#Template Class Requirements',
'simplevector.htm#Elements of Specified Type',
'simplevector.htm#Default Constructor',
'simplevector.htm#Sizing Constructor',
'simplevector.htm#Copy Constructor',
'simplevector.htm#Element Constructor and Destructor',
'simplevector.htm#Assignment',
'simplevector.htm#size',
'simplevector.htm#resize',
'simplevector.htm#value_type',
'simplevector.htm#Element Access',
'simplevector.htm#Element Access.Using Value',
'simplevector.htm#Element Access.Assignment',
'simplevector.htm#Example',
'simplevector.htm#Exercise'
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
