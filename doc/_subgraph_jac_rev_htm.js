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
'subgraph_jac_rev.htm'
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
'sparse_jac.htm',
'sparse_jacobian.htm',
'sparse_hes.htm',
'sparse_hessian.htm',
'subgraph_jac_rev.htm'
];
var list_down0 = [
'subgraph_jac_rev.cpp.htm',
'subgraph_hes2jac.cpp.htm'
];
var list_current0 = [
'subgraph_jac_rev.htm#Syntax',
'subgraph_jac_rev.htm#See Also',
'subgraph_jac_rev.htm#Purpose',
'subgraph_jac_rev.htm#Method',
'subgraph_jac_rev.htm#BaseVector',
'subgraph_jac_rev.htm#SizeVector',
'subgraph_jac_rev.htm#BoolVector',
'subgraph_jac_rev.htm#f',
'subgraph_jac_rev.htm#x',
'subgraph_jac_rev.htm#Uses Forward',
'subgraph_jac_rev.htm#subset',
'subgraph_jac_rev.htm#select_domain',
'subgraph_jac_rev.htm#select_range',
'subgraph_jac_rev.htm#matrix_out',
'subgraph_jac_rev.htm#Example'
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
