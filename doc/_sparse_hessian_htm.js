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
'sparse_hessian.htm'
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
'sparse_hessian.cpp.htm',
'sub_sparse_hes.cpp.htm',
'sparse_sub_hes.cpp.htm'
];
var list_current0 = [
'sparse_hessian.htm#Syntax',
'sparse_hessian.htm#Purpose',
'sparse_hessian.htm#f',
'sparse_hessian.htm#x',
'sparse_hessian.htm#w',
'sparse_hessian.htm#p',
'sparse_hessian.htm#p.Purpose',
'sparse_hessian.htm#p.work',
'sparse_hessian.htm#p.Column Subset',
'sparse_hessian.htm#row, col',
'sparse_hessian.htm#hes',
'sparse_hessian.htm#work',
'sparse_hessian.htm#work.color_method',
'sparse_hessian.htm#work.colpack.star Deprecated 2017-06-01',
'sparse_hessian.htm#work.p',
'sparse_hessian.htm#n_sweep',
'sparse_hessian.htm#BaseVector',
'sparse_hessian.htm#SetVector',
'sparse_hessian.htm#SetVector.Restrictions',
'sparse_hessian.htm#SizeVector',
'sparse_hessian.htm#Uses Forward',
'sparse_hessian.htm#Example',
'sparse_hessian.htm#Subset Hessian'
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
