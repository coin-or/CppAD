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
'sparse_rc.htm'
];
var list_current0 = [
'sparse_rc.htm#Syntax',
'sparse_rc.htm#SizeVector',
'sparse_rc.htm#empty',
'sparse_rc.htm#pattern',
'sparse_rc.htm#other',
'sparse_rc.htm#other.Assignment and Constructor',
'sparse_rc.htm#other.Move Semantics Assignment and Constructor',
'sparse_rc.htm#other.swap',
'sparse_rc.htm#nr',
'sparse_rc.htm#nc',
'sparse_rc.htm#nnz',
'sparse_rc.htm#resize',
'sparse_rc.htm#set',
'sparse_rc.htm#set.k',
'sparse_rc.htm#set.r',
'sparse_rc.htm#set.c',
'sparse_rc.htm#row',
'sparse_rc.htm#col',
'sparse_rc.htm#row_major',
'sparse_rc.htm#col_major',
'sparse_rc.htm#Example'
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
