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
'atomic_two.htm',
'atomic_two_forward.htm'
];
var list_current0 = [
'atomic_two_forward.htm#Syntax',
'atomic_two_forward.htm#Syntax.Base',
'atomic_two_forward.htm#Syntax.AD&lt;Base&gt;',
'atomic_two_forward.htm#Purpose',
'atomic_two_forward.htm#Implementation',
'atomic_two_forward.htm#p',
'atomic_two_forward.htm#q',
'atomic_two_forward.htm#vx',
'atomic_two_forward.htm#vy',
'atomic_two_forward.htm#tx',
'atomic_two_forward.htm#atx',
'atomic_two_forward.htm#ty',
'atomic_two_forward.htm#aty',
'atomic_two_forward.htm#ok',
'atomic_two_forward.htm#Discussion'
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
