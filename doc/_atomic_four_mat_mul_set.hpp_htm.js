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
'atomic_four.htm',
'atomic_four_example.htm',
'atomic_mat_mul.htm',
'atomic_mat_mul_implement.htm',
'atomic_four_mat_mul_set.hpp.htm'
];
var list_current0 = [
'atomic_four_mat_mul_set.hpp.htm#Syntax',
'atomic_four_mat_mul_set.hpp.htm#Prototype',
'atomic_four_mat_mul_set.hpp.htm#Purpose',
'atomic_four_mat_mul_set.hpp.htm#n_left',
'atomic_four_mat_mul_set.hpp.htm#n_middle',
'atomic_four_mat_mul_set.hpp.htm#n_right',
'atomic_four_mat_mul_set.hpp.htm#call_id',
'atomic_four_mat_mul_set.hpp.htm#Source'
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
