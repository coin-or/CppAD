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
'abs_normal.htm',
'abs_min_quad.htm'
];
var list_down3 = [
'install.htm',
'introduction.htm',
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
'record_adfun.htm',
'drivers.htm',
'forward.htm',
'reverse.htm',
'sparsity_pattern.htm',
'sparse_derivative.htm',
'optimize.htm',
'abs_normal.htm',
'funcheck.htm',
'check_for_nan.htm'
];
var list_down1 = [
'abs_normal_fun.htm',
'abs_print_mat.htm',
'abs_eval.htm',
'simplex_method.htm',
'lp_box.htm',
'abs_min_linear.htm',
'min_nso_linear.htm',
'qp_interior.htm',
'qp_box.htm',
'abs_min_quad.htm',
'min_nso_quad.htm'
];
var list_down0 = [
'abs_min_quad.cpp.htm',
'abs_min_quad.hpp.htm'
];
var list_current0 = [
'abs_min_quad.htm#Syntax',
'abs_min_quad.htm#Prototype',
'abs_min_quad.htm#Source',
'abs_min_quad.htm#Purpose',
'abs_min_quad.htm#DblVector',
'abs_min_quad.htm#SizeVector',
'abs_min_quad.htm#f',
'abs_min_quad.htm#level',
'abs_min_quad.htm#n',
'abs_min_quad.htm#m',
'abs_min_quad.htm#s',
'abs_min_quad.htm#g',
'abs_min_quad.htm#g_hat',
'abs_min_quad.htm#g_jac',
'abs_min_quad.htm#hessian',
'abs_min_quad.htm#bound',
'abs_min_quad.htm#epsilon',
'abs_min_quad.htm#maxitr',
'abs_min_quad.htm#delta_x',
'abs_min_quad.htm#Method',
'abs_min_quad.htm#Method.sigma',
'abs_min_quad.htm#Method.Cutting Planes',
'abs_min_quad.htm#Method.Iteration',
'abs_min_quad.htm#Example'
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
