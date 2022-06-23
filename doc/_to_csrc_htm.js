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
'to_csrc.htm#c_type',
'to_csrc.htm#JIT Functions',
'to_csrc.htm#Atomic Callbacks',
'to_csrc.htm#call_id',
'to_csrc.htm#nx',
'to_csrc.htm#x',
'to_csrc.htm#ny',
'to_csrc.htm#y',
'to_csrc.htm#compare_change',
'to_csrc.htm#flag',
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
