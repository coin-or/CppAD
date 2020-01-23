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
'json_ad_graph.htm',
'json_get_started.cpp.htm'
];
var list_current0 = [
'json_get_started.cpp.htm#Notation',
'json_get_started.cpp.htm#Node Table',
'json_get_started.cpp.htm#Include',
'json_get_started.cpp.htm#Syntax',
'json_get_started.cpp.htm#Setup',
'json_get_started.cpp.htm#Function',
'json_get_started.cpp.htm#Function.Begin Function',
'json_get_started.cpp.htm#Function.Begin op_define_vec',
'json_get_started.cpp.htm#Function.Define Unary',
'json_get_started.cpp.htm#Function.Define Sum',
'json_get_started.cpp.htm#Function.End op_define_vec',
'json_get_started.cpp.htm#Function.n_dynamic_ind',
'json_get_started.cpp.htm#Function.n_variable_ind',
'json_get_started.cpp.htm#Function.constant_vec',
'json_get_started.cpp.htm#Function.Begin op_usage_vec',
'json_get_started.cpp.htm#Function.op_usage',
'json_get_started.cpp.htm#Function.End op_usage_vec',
'json_get_started.cpp.htm#Function.dependent_vec',
'json_get_started.cpp.htm#Function.End Function',
'json_get_started.cpp.htm#Convert Single to Double Quotes',
'json_get_started.cpp.htm#double f(x, p)',
'json_get_started.cpp.htm#Check f(x, p)',
'json_get_started.cpp.htm#AD&lt;double&gt; f(x, p)',
'json_get_started.cpp.htm#Evaluate Derivative',
'json_get_started.cpp.htm#double g(x, p) = d/dx f(x, p)',
'json_get_started.cpp.htm#Convert to Json and Back',
'json_get_started.cpp.htm#Check g(x, p)'
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
