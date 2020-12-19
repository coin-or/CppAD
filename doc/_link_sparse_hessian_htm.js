var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'speed.htm',
'speed_main.htm',
'link_routines.htm',
'link_sparse_hessian.htm'
];
var list_current0 = [
'link_sparse_hessian.htm#Syntax',
'link_sparse_hessian.htm#Prototype',
'link_sparse_hessian.htm#Namespace',
'link_sparse_hessian.htm#Method',
'link_sparse_hessian.htm#Sparsity Pattern',
'link_sparse_hessian.htm#size',
'link_sparse_hessian.htm#repeat',
'link_sparse_hessian.htm#x',
'link_sparse_hessian.htm#row',
'link_sparse_hessian.htm#col',
'link_sparse_hessian.htm#Row Major',
'link_sparse_hessian.htm#Lower Triangular',
'link_sparse_hessian.htm#hessian',
'link_sparse_hessian.htm#n_color',
'link_sparse_hessian.htm#n_color.double'
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
