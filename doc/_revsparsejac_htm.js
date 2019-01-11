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
'revsparsejac.htm'
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
'for_jac_sparsity.htm',
'rev_jac_sparsity.htm',
'for_hes_sparsity.htm',
'rev_hes_sparsity.htm',
'subgraph_sparsity.htm',
'dependency.cpp.htm',
'rc_sparsity.cpp.htm',
'forsparsejac.htm',
'revsparsejac.htm',
'revsparsehes.htm',
'forsparsehes.htm'
];
var list_down0 = [
'rev_sparse_jac.cpp.htm'
];
var list_current0 = [
'revsparsejac.htm#Syntax',
'revsparsejac.htm#Purpose',
'revsparsejac.htm#f',
'revsparsejac.htm#x',
'revsparsejac.htm#q',
'revsparsejac.htm#transpose',
'revsparsejac.htm#dependency',
'revsparsejac.htm#r',
'revsparsejac.htm#r.transpose false',
'revsparsejac.htm#r.transpose true',
'revsparsejac.htm#s',
'revsparsejac.htm#s.transpose false',
'revsparsejac.htm#s.transpose true',
'revsparsejac.htm#SetVector',
'revsparsejac.htm#Entire Sparsity Pattern',
'revsparsejac.htm#Example'
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
