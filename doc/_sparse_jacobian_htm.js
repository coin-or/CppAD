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
'sparse_derivative.htm',
'sparse_jacobian.htm'
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
'sparse_jac.htm',
'sparse_jacobian.htm',
'sparse_hes.htm',
'sparse_hessian.htm',
'subgraph_jac_rev.htm'
];
var list_down0 = [
'sparse_jacobian.cpp.htm'
];
var list_current0 = [
'sparse_jacobian.htm#Syntax',
'sparse_jacobian.htm#Purpose',
'sparse_jacobian.htm#f',
'sparse_jacobian.htm#x',
'sparse_jacobian.htm#p',
'sparse_jacobian.htm#row, col',
'sparse_jacobian.htm#jac',
'sparse_jacobian.htm#work',
'sparse_jacobian.htm#work.color_method',
'sparse_jacobian.htm#work.p',
'sparse_jacobian.htm#n_sweep',
'sparse_jacobian.htm#VectorBase',
'sparse_jacobian.htm#VectorSet',
'sparse_jacobian.htm#VectorSet.Restrictions',
'sparse_jacobian.htm#VectorSize',
'sparse_jacobian.htm#Uses Forward',
'sparse_jacobian.htm#Example'
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
