var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'ad.htm',
'advalued.htm',
'atomic.htm',
'atomic_base.htm',
'atomic_for_sparse_hes.htm',
'atomic_for_sparse_hes.cpp.htm'
];
var list_down3 = [
'checkpoint.htm',
'atomic_base.htm',
'atomic_example.htm'
];
var list_down2 = [
'atomic_ctor.htm',
'atomic_option.htm',
'atomic_afun.htm',
'atomic_forward.htm',
'atomic_reverse.htm',
'atomic_for_sparse_jac.htm',
'atomic_rev_sparse_jac.htm',
'atomic_for_sparse_hes.htm',
'atomic_rev_sparse_hes.htm',
'atomic_base_clear.htm'
];
var list_down1 = [
'atomic_for_sparse_hes.cpp.htm'
];
var list_current0 = [
'atomic_for_sparse_hes.cpp.htm#Purpose',
'atomic_for_sparse_hes.cpp.htm#function',
'atomic_for_sparse_hes.cpp.htm#Start Class Definition',
'atomic_for_sparse_hes.cpp.htm#Constructor',
'atomic_for_sparse_hes.cpp.htm#forward',
'atomic_for_sparse_hes.cpp.htm#for_sparse_jac',
'atomic_for_sparse_hes.cpp.htm#rev_sparse_jac',
'atomic_for_sparse_hes.cpp.htm#for_sparse_hes',
'atomic_for_sparse_hes.cpp.htm#Use Atomic Function',
'atomic_for_sparse_hes.cpp.htm#Test with x_1 Both a Variable and a Parameter'
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
