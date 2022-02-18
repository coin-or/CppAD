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
'atomic.htm',
'atomic_three.htm',
'atomic_three_define.htm'
];
var list_current0 = [
'atomic_three_define.htm#Syntax',
'atomic_three_define.htm#Syntax.Define Class',
'atomic_three_define.htm#Syntax.Construct Atomic Function',
'atomic_three_define.htm#Syntax.Use Atomic Function',
'atomic_three_define.htm#Syntax.Class Member Callbacks',
'atomic_three_define.htm#See Also',
'atomic_three_define.htm#Purpose',
'atomic_three_define.htm#Purpose.Speed',
'atomic_three_define.htm#Purpose.Reduce Memory',
'atomic_three_define.htm#ad_type',
'atomic_three_define.htm#Virtual Functions',
'atomic_three_define.htm#Base',
'atomic_three_define.htm#parameter_x',
'atomic_three_define.htm#parameter_x.Constant',
'atomic_three_define.htm#parameter_x.Dynamic',
'atomic_three_define.htm#parameter_x.Variable',
'atomic_three_define.htm#type_x',
'atomic_three_define.htm#Contents'
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
