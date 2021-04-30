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
'sparse_rcv.htm'
];
var list_current0 = [
'sparse_rcv.htm#Syntax',
'sparse_rcv.htm#SizeVector',
'sparse_rcv.htm#ValueVector',
'sparse_rcv.htm#empty',
'sparse_rcv.htm#pattern',
'sparse_rcv.htm#matrix',
'sparse_rcv.htm#other',
'sparse_rcv.htm#other.Assignment and Constructor',
'sparse_rcv.htm#other.Move Semantics Assignment and Constructor',
'sparse_rcv.htm#other.swap',
'sparse_rcv.htm#nr',
'sparse_rcv.htm#nc',
'sparse_rcv.htm#nnz',
'sparse_rcv.htm#set',
'sparse_rcv.htm#set.k',
'sparse_rcv.htm#set.v',
'sparse_rcv.htm#row',
'sparse_rcv.htm#col',
'sparse_rcv.htm#val',
'sparse_rcv.htm#pat',
'sparse_rcv.htm#row_major',
'sparse_rcv.htm#col_major',
'sparse_rcv.htm#Eigen Matrix',
'sparse_rcv.htm#Example'
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
