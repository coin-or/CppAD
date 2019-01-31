var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'appendix.htm',
'deprecated.htm',
'chkpoint_one.htm'
];
var list_current0 = [
'chkpoint_one.htm#Deprecated 2019-01-14',
'chkpoint_one.htm#Syntax',
'chkpoint_one.htm#See Also',
'chkpoint_one.htm#Purpose',
'chkpoint_one.htm#Purpose.Reduce Memory',
'chkpoint_one.htm#Purpose.Faster Recording',
'chkpoint_one.htm#Purpose.Repeating Forward',
'chkpoint_one.htm#Purpose.Restriction',
'chkpoint_one.htm#Purpose.Multiple Level AD',
'chkpoint_one.htm#Method',
'chkpoint_one.htm#constructor',
'chkpoint_one.htm#Base',
'chkpoint_one.htm#ADVector',
'chkpoint_one.htm#name',
'chkpoint_one.htm#ax',
'chkpoint_one.htm#ay',
'chkpoint_one.htm#sparsity',
'chkpoint_one.htm#optimize',
'chkpoint_one.htm#size_var',
'chkpoint_one.htm#option',
'chkpoint_one.htm#algo',
'chkpoint_one.htm#atom_fun',
'chkpoint_one.htm#Memory',
'chkpoint_one.htm#Memory.Restriction',
'chkpoint_one.htm#Parallel Mode',
'chkpoint_one.htm#Parallel Mode.clear'
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
