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
var list_current0 = [
'wish_list.htm#See Also',
'wish_list.htm#Purpose',
'wish_list.htm#Dynamic Parameters',
'wish_list.htm#Graph Operators',
'wish_list.htm#Reverse Mode',
'wish_list.htm#Atomic Examples',
'wish_list.htm#Abs-normal',
'wish_list.htm#Abs-normal.Atomic Functions',
'wish_list.htm#Abs-normal.Return Functions',
'wish_list.htm#Abs-normal.Cancellation',
'wish_list.htm#cppad_lib',
'wish_list.htm#cppad_lib.Requirement',
'wish_list.htm#cppad_lib.inline',
'wish_list.htm#cppad_lib.Compilation Speed',
'wish_list.htm#checkpoint',
'wish_list.htm#checkpoint.Tapeless AD',
'wish_list.htm#checkpoint.Re-taping',
'wish_list.htm#checkpoint.Testing',
'wish_list.htm#Subgraph',
'wish_list.htm#Subgraph.Forward Mode',
'wish_list.htm#Subgraph.Sparsity',
'wish_list.htm#Element-wise Atomic Operations',
'wish_list.htm#check_finite',
'wish_list.htm#test_boolofvoid',
'wish_list.htm#Example',
'wish_list.htm#Optimization',
'wish_list.htm#Optimization.Taping',
'wish_list.htm#Optimization.Special Operators',
'wish_list.htm#Base Requirements',
'wish_list.htm#Adolc',
'wish_list.htm#Forward Mode Recomputation',
'wish_list.htm#Iterator Interface',
'wish_list.htm#Tracing',
'wish_list.htm#atan2'
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
