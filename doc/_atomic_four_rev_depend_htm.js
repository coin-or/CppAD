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
'atomic_four.htm',
'atomic_four_define.htm',
'atomic_four_rev_depend.htm'
];
var list_current0 = [
'atomic_four_rev_depend.htm#Syntax',
'atomic_four_rev_depend.htm#Syntax.Prototype',
'atomic_four_rev_depend.htm#Dependency Analysis',
'atomic_four_rev_depend.htm#Implementation',
'atomic_four_rev_depend.htm#Base',
'atomic_four_rev_depend.htm#vector',
'atomic_four_rev_depend.htm#call_id',
'atomic_four_rev_depend.htm#depend_x',
'atomic_four_rev_depend.htm#depend_x.Optimize',
'atomic_four_rev_depend.htm#depend_y',
'atomic_four_rev_depend.htm#ok',
'atomic_four_rev_depend.htm#Example'
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
