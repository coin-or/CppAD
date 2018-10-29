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
'speed_double.htm'
];
var list_down2 = [
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
var list_down1 = [
'speed_main.htm',
'speed_utility.htm',
'speed_double.htm',
'speed_adolc.htm',
'speed_cppad.htm',
'speed_fadbad.htm',
'speed_sacado.htm'
];
var list_down0 = [
'double_det_minor.cpp.htm',
'double_det_lu.cpp.htm',
'double_mat_mul.cpp.htm',
'double_ode.cpp.htm',
'double_poly.cpp.htm',
'double_sparse_hessian.cpp.htm',
'double_sparse_jacobian.cpp.htm'
];
var list_current0 = [
'speed_double.htm#Purpose',
'speed_double.htm#Running Tests',
'speed_double.htm#Contents'
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
