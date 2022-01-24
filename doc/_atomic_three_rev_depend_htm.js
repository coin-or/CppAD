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
'atomic_three_rev_depend.htm'
];
var list_current0 = [
'atomic_three_rev_depend.htm#Syntax',
'atomic_three_rev_depend.htm#Syntax.Prototype',
'atomic_three_rev_depend.htm#Dependency Analysis',
'atomic_three_rev_depend.htm#Implementation',
'atomic_three_rev_depend.htm#Base',
'atomic_three_rev_depend.htm#parameter_x',
'atomic_three_rev_depend.htm#type_x',
'atomic_three_rev_depend.htm#depend_x',
'atomic_three_rev_depend.htm#depend_x.Optimize',
'atomic_three_rev_depend.htm#depend_y',
'atomic_three_rev_depend.htm#ok',
'atomic_three_rev_depend.htm#Contents',
'atomic_three_rev_depend.htm#Example'
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
