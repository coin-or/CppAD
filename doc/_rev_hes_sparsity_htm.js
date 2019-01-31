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
'rev_hes_sparsity.htm'
];
var list_current0 = [
'rev_hes_sparsity.htm#Syntax',
'rev_hes_sparsity.htm#Purpose',
'rev_hes_sparsity.htm#x',
'rev_hes_sparsity.htm#BoolVector',
'rev_hes_sparsity.htm#SizeVector',
'rev_hes_sparsity.htm#f',
'rev_hes_sparsity.htm#R',
'rev_hes_sparsity.htm#select_range',
'rev_hes_sparsity.htm#transpose',
'rev_hes_sparsity.htm#internal_bool',
'rev_hes_sparsity.htm#pattern_out',
'rev_hes_sparsity.htm#Sparsity for Entire Hessian',
'rev_hes_sparsity.htm#Example'
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
