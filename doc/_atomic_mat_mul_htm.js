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
'atomic_four_example.htm',
'atomic_mat_mul.htm'
];
var list_current0 = [
'atomic_mat_mul.htm#Under Construction',
'atomic_mat_mul.htm#Syntax',
'atomic_mat_mul.htm#Purpose',
'atomic_mat_mul.htm#n_left',
'atomic_mat_mul.htm#n_middle',
'atomic_mat_mul.htm#n_right',
'atomic_mat_mul.htm#call_id',
'atomic_mat_mul.htm#x',
'atomic_mat_mul.htm#y',
'atomic_mat_mul.htm#Theory',
'atomic_mat_mul.htm#Theory.Forward',
'atomic_mat_mul.htm#Theory.Matrix Argument Scalar Valued Function',
'atomic_mat_mul.htm#Theory.Reverse',
'atomic_mat_mul.htm#Contents'
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
