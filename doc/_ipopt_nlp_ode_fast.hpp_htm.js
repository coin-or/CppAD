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
'deprecated.htm',
'cppad_ipopt_nlp.htm',
'ipopt_nlp_ode.htm',
'ipopt_nlp_ode_fast.htm',
'ipopt_nlp_ode_fast.hpp.htm'
];
var list_down3 = [
'ipopt_nlp_get_started.cpp.htm',
'ipopt_nlp_ode.htm',
'ipopt_ode_speed.cpp.htm'
];
var list_down2 = [
'ipopt_nlp_ode_problem.htm',
'ipopt_nlp_ode_simple.htm',
'ipopt_nlp_ode_fast.htm',
'ipopt_nlp_ode_run.hpp.htm',
'ipopt_nlp_ode_check.cpp.htm'
];
var list_down1 = [
'ipopt_nlp_ode_fast.hpp.htm'
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
