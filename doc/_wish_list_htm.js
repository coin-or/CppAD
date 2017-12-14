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
'wish_list.htm'
];
var list_down2 = [
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
var list_down1 = [
'faq.htm',
'directory.htm',
'theory.htm',
'glossary.htm',
'bib.htm',
'wish_list.htm',
'whats_new.htm',
'deprecated.htm',
'compare_c.htm',
'numeric_ad.htm',
'addon.htm',
'license.htm'
];
var list_current0 = [
'wish_list.htm#Multi-Threading',
'wish_list.htm#Atomic',
'wish_list.htm#Atomic.Subgraph',
'wish_list.htm#Atomic.New API',
'wish_list.htm#Atomic.Sparsity',
'wish_list.htm#Atomic.Element-wise Operations',
'wish_list.htm#check_finite',
'wish_list.htm#test_boolofvoid',
'wish_list.htm#Eigen',
'wish_list.htm#Example',
'wish_list.htm#Optimization',
'wish_list.htm#Optimization.Taping',
'wish_list.htm#Optimization.Special Operators',
'wish_list.htm#Optimization.Memory',
'wish_list.htm#checkpoint',
'wish_list.htm#checkpoint.Retape',
'wish_list.htm#checkpoint.Testing',
'wish_list.htm#Compilation Speed',
'wish_list.htm#Base Requirements',
'wish_list.htm#Adolc',
'wish_list.htm#Forward Mode Recomputation',
'wish_list.htm#Iterator Interface',
'wish_list.htm#Operation Sequence',
'wish_list.htm#Software Guidelines',
'wish_list.htm#Tracing',
'wish_list.htm#atan2',
'wish_list.htm#BenderQuad'
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
