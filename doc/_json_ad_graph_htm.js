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
'json_ad_graph.htm'
];
var list_current0 = [
'json_ad_graph.htm#Under Construction',
'json_ad_graph.htm#Format',
'json_ad_graph.htm#Token',
'json_ad_graph.htm#Token.White Space',
'json_ad_graph.htm#Token.Non-Negative Integer',
'json_ad_graph.htm#Token.Floating Point Number',
'json_ad_graph.htm#Token.String',
'json_ad_graph.htm#Token.Single Character',
'json_ad_graph.htm#operator',
'json_ad_graph.htm#operator.code',
'json_ad_graph.htm#operator.n_results',
'json_ad_graph.htm#operator.n_arg',
'json_ad_graph.htm#operator.Arguments',
'json_ad_graph.htm#operator.name',
'json_ad_graph.htm#dynamic_ind_vec',
'json_ad_graph.htm#dynamic_ind_vec.Node Indices',
'json_ad_graph.htm#independent_vec',
'json_ad_graph.htm#independent_vec.Node Indices',
'json_ad_graph.htm#string_vec',
'json_ad_graph.htm#string_vec.Node Indices',
'json_ad_graph.htm#constant_vec',
'json_ad_graph.htm#constant_vec.Node Indices',
'json_ad_graph.htm#operator_vec',
'json_ad_graph.htm#operator_vec.Node Indices',
'json_ad_graph.htm#dependent_vec',
'json_ad_graph.htm#AD Graph'
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
