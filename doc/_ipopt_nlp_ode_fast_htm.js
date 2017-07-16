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
'ipopt_nlp_ode_fast.htm'
];
var list_down3 = [
'include_deprecated.htm',
'fundeprecated.htm',
'comparechange.htm',
'omp_max_thread.htm',
'tracknewdel.htm',
'omp_alloc.htm',
'memory_leak.htm',
'epsilon.htm',
'test_vector.htm',
'cppad_ipopt_nlp.htm',
'old_atomic.htm',
'zdouble.htm',
'autotools.htm'
];
var list_down2 = [
'ipopt_nlp_get_started.cpp.htm',
'ipopt_nlp_ode.htm',
'ipopt_ode_speed.cpp.htm'
];
var list_down1 = [
'ipopt_nlp_ode_problem.htm',
'ipopt_nlp_ode_simple.htm',
'ipopt_nlp_ode_fast.htm',
'ipopt_nlp_ode_run.hpp.htm',
'ipopt_nlp_ode_check.cpp.htm'
];
var list_down0 = [
'ipopt_nlp_ode_fast.hpp.htm'
];
var list_current0 = [
'ipopt_nlp_ode_fast.htm#Purpose',
'ipopt_nlp_ode_fast.htm#Objective Function',
'ipopt_nlp_ode_fast.htm#Objective Function.Range Indices I(k,0)',
'ipopt_nlp_ode_fast.htm#Objective Function.Domain Indices J(k,0)',
'ipopt_nlp_ode_fast.htm#Initial Condition',
'ipopt_nlp_ode_fast.htm#Initial Condition.Range Indices I(k,0)',
'ipopt_nlp_ode_fast.htm#Initial Condition.Domain Indices J(k,0)',
'ipopt_nlp_ode_fast.htm#Trapezoidal Approximation',
'ipopt_nlp_ode_fast.htm#Trapezoidal Approximation.Range Indices I(k,0)',
'ipopt_nlp_ode_fast.htm#Trapezoidal Approximation.Domain Indices J(k,0)',
'ipopt_nlp_ode_fast.htm#Source'
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
