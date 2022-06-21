var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'adfun.htm',
'to_csrc.htm'
];
var list_current0 = [
'to_csrc.htm#Syntax',
'to_csrc.htm#Prototype',
'to_csrc.htm#fun',
'to_csrc.htm#Base',
'to_csrc.htm#RecBase',
'to_csrc.htm#os',
'to_csrc.htm#type',
'to_csrc.htm#C Function',
'to_csrc.htm#C Function.Syntax',
'to_csrc.htm#C Function.Function Type',
'to_csrc.htm#C Function.call_id',
'to_csrc.htm#C Function.nx',
'to_csrc.htm#C Function.x',
'to_csrc.htm#C Function.ny',
'to_csrc.htm#C Function.y',
'to_csrc.htm#C Function.compare_change',
'to_csrc.htm#C Function.flag',
'to_csrc.htm#Restrictions',
'to_csrc.htm#Example'
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
