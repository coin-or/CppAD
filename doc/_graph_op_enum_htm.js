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
'other_adfun.htm',
'cpp_ad_graph.htm',
'graph_op_enum.htm'
];
var list_current0 = [
'graph_op_enum.htm#Unary',
'graph_op_enum.htm#Unary.Require C++11',
'graph_op_enum.htm#Binary',
'graph_op_enum.htm#Conditional Expression',
'graph_op_enum.htm#Conditional Expression.Other Comparisons',
'graph_op_enum.htm#Comparison',
'graph_op_enum.htm#Comparison.Other Comparisons',
'graph_op_enum.htm#Summation',
'graph_op_enum.htm#Discrete Function',
'graph_op_enum.htm#Atomic Function',
'graph_op_enum.htm#Print',
'graph_op_enum.htm#Missing Operators',
'graph_op_enum.htm#Enum Values',
'graph_op_enum.htm#Examples',
'graph_op_enum.htm#Contents'
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
