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
'abs_normal_fun.htm'
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
'abs_get_started.cpp.htm'
];
var list_current0 = [
'abs_normal_fun.htm#Syntax',
'abs_normal_fun.htm#f',
'abs_normal_fun.htm#f.n',
'abs_normal_fun.htm#f.m',
'abs_normal_fun.htm#f.s',
'abs_normal_fun.htm#a',
'abs_normal_fun.htm#a.zeta',
'abs_normal_fun.htm#a.a(x)',
'abs_normal_fun.htm#g',
'abs_normal_fun.htm#g.z(x, u)',
'abs_normal_fun.htm#g.y(x, u)',
'abs_normal_fun.htm#Affine Approximation',
'abs_normal_fun.htm#Abs-normal Approximation',
'abs_normal_fun.htm#Abs-normal Approximation.Approximating a(x)',
'abs_normal_fun.htm#Abs-normal Approximation.Approximating f(x)',
'abs_normal_fun.htm#Correspondence to Literature',
'abs_normal_fun.htm#Example'
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
