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
'cpp_ad_graph.htm'
];
var list_current0 = [
'cpp_ad_graph.htm#See Also',
'cpp_ad_graph.htm#function',
'cpp_ad_graph.htm#Node Indices',
'cpp_ad_graph.htm#Node Indices.p',
'cpp_ad_graph.htm#Node Indices.x',
'cpp_ad_graph.htm#Node Indices.c',
'cpp_ad_graph.htm#Node Indices.r',
'cpp_ad_graph.htm#function_name',
'cpp_ad_graph.htm#discrete_name_vec',
'cpp_ad_graph.htm#atomic_name_vec',
'cpp_ad_graph.htm#print_text_vec',
'cpp_ad_graph.htm#n_dynamic_ind',
'cpp_ad_graph.htm#n_variable_ind',
'cpp_ad_graph.htm#constant_vec',
'cpp_ad_graph.htm#operator_vec',
'cpp_ad_graph.htm#operator_arg',
'cpp_ad_graph.htm#operator_arg.print_graph_op',
'cpp_ad_graph.htm#operator_arg.discrete_graph_op',
'cpp_ad_graph.htm#operator_arg.atom_graph_op',
'cpp_ad_graph.htm#operator_arg.sum_graph_op',
'cpp_ad_graph.htm#dependent_vec',
'cpp_ad_graph.htm#cpp_graph',
'cpp_ad_graph.htm#Contents'
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
