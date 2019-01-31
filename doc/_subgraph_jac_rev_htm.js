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
'sparse_derivative.htm',
'subgraph_jac_rev.htm'
];
var list_current0 = [
'subgraph_jac_rev.htm#Syntax',
'subgraph_jac_rev.htm#See Also',
'subgraph_jac_rev.htm#Purpose',
'subgraph_jac_rev.htm#Method',
'subgraph_jac_rev.htm#BaseVector',
'subgraph_jac_rev.htm#SizeVector',
'subgraph_jac_rev.htm#BoolVector',
'subgraph_jac_rev.htm#f',
'subgraph_jac_rev.htm#x',
'subgraph_jac_rev.htm#Uses Forward',
'subgraph_jac_rev.htm#subset',
'subgraph_jac_rev.htm#select_domain',
'subgraph_jac_rev.htm#select_range',
'subgraph_jac_rev.htm#matrix_out',
'subgraph_jac_rev.htm#Example'
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
