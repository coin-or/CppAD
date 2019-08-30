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
'json_ad_graph.htm#op_define_vec',
'json_ad_graph.htm#op_define_vec.op_define',
'json_ad_graph.htm#op_define_vec.op_code',
'json_ad_graph.htm#op_define_vec.n_arg',
'json_ad_graph.htm#op_define_vec.n_result',
'json_ad_graph.htm#op_usage',
'json_ad_graph.htm#op_usage.n_arg Not in Usage',
'json_ad_graph.htm#op_usage.n_arg In Usage',
'json_ad_graph.htm#op_usage.n_arg And Strings In Usage',
'json_ad_graph.htm#op_usage.first_arg, ..., last_arg',
'json_ad_graph.htm#dynamic_ind_vec',
'json_ad_graph.htm#dynamic_ind_vec.n_dynamic_ind',
'json_ad_graph.htm#dynamic_ind_vec.Node Indices',
'json_ad_graph.htm#independent_vec',
'json_ad_graph.htm#independent_vec.n_independent',
'json_ad_graph.htm#independent_vec.Node Indices',
'json_ad_graph.htm#string_vec',
'json_ad_graph.htm#string_vec.n_string',
'json_ad_graph.htm#string_vec.Node Indices',
'json_ad_graph.htm#constant_vec',
'json_ad_graph.htm#constant_vec.n_constant',
'json_ad_graph.htm#constant_vec.Node Indices',
'json_ad_graph.htm#op_usage_vec',
'json_ad_graph.htm#op_usage_vec.n_usage',
'json_ad_graph.htm#op_usage_vec.Result Indices',
'json_ad_graph.htm#dependent_vec',
'json_ad_graph.htm#dependent_vec.n_dependent',
'json_ad_graph.htm#AD Graph',
'json_ad_graph.htm#AD Graph.function_name',
'json_ad_graph.htm#AD Graph.function',
'json_ad_graph.htm#Contents'
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
