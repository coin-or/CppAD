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
'vecad.htm'
];
var list_current0 = [
'vecad.htm#Syntax',
'vecad.htm#Purpose',
'vecad.htm#Alternatives',
'vecad.htm#Efficiency',
'vecad.htm#VecAD&lt;Base&gt;::reference',
'vecad.htm#VecAD&lt;Base&gt;::reference.Exceptions',
'vecad.htm#Constructor',
'vecad.htm#Constructor.vec',
'vecad.htm#Constructor.n',
'vecad.htm#size',
'vecad.htm#Base Indexing',
'vecad.htm#Base Indexing.i',
'vecad.htm#Base Indexing.base',
'vecad.htm#AD Indexing',
'vecad.htm#AD Indexing.ind',
'vecad.htm#AD Indexing.result',
'vecad.htm#AD Indexing.right',
'vecad.htm#AD Indexing.left',
'vecad.htm#Example',
'vecad.htm#Speed and Memory'
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
