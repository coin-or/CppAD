var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'speed.htm',
'speed_utility.htm',
'ode_evaluate.htm'
];
var list_down3 = [
'install.htm',
'theory.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down2 = [
'speed_main.htm',
'speed_utility.htm',
'speed_double.htm',
'speed_adolc.htm',
'speed_cppad.htm',
'speed_fadbad.htm',
'speed_sacado.htm'
];
var list_down1 = [
'det_by_lu.htm',
'det_of_minor.htm',
'det_by_minor.htm',
'det_33.htm',
'det_grad_33.htm',
'mat_sum_sq.htm',
'ode_evaluate.htm',
'sparse_jac_fun.htm',
'sparse_hes_fun.htm',
'uniform_01.htm'
];
var list_down0 = [
'ode_evaluate.cpp.htm',
'ode_evaluate.hpp.htm'
];
var list_current0 = [
'ode_evaluate.htm#Syntax',
'ode_evaluate.htm#Purpose',
'ode_evaluate.htm#Inclusion',
'ode_evaluate.htm#Float',
'ode_evaluate.htm#Float.Operation Sequence',
'ode_evaluate.htm#Float.fabs',
'ode_evaluate.htm#x',
'ode_evaluate.htm#p',
'ode_evaluate.htm#p.p == 0',
'ode_evaluate.htm#p.p = 1',
'ode_evaluate.htm#fp',
'ode_evaluate.htm#fp.Function',
'ode_evaluate.htm#fp.Gradient',
'ode_evaluate.htm#Example',
'ode_evaluate.htm#Source Code'
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
