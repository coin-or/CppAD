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
'sparsity_pattern.htm',
'subgraph_sparsity.htm'
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
'for_jac_sparsity.htm',
'forsparsejac.htm',
'rev_jac_sparsity.htm',
'revsparsejac.htm',
'rev_hes_sparsity.htm',
'revsparsehes.htm',
'for_hes_sparsity.htm',
'forsparsehes.htm',
'dependency.cpp.htm',
'rc_sparsity.cpp.htm',
'subgraph_sparsity.htm'
];
var list_down0 = [
'subgraph_sparsity.cpp.htm'
];
var list_current0 = [
'subgraph_sparsity.htm#Syntax',
'subgraph_sparsity.htm#Notation',
'subgraph_sparsity.htm#Method',
'subgraph_sparsity.htm#Atomic Function',
'subgraph_sparsity.htm#BoolVector',
'subgraph_sparsity.htm#SizeVector',
'subgraph_sparsity.htm#f',
'subgraph_sparsity.htm#select_domain',
'subgraph_sparsity.htm#select_range',
'subgraph_sparsity.htm#transpose',
'subgraph_sparsity.htm#pattern_out',
'subgraph_sparsity.htm#Example'
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
