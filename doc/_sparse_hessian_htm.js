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
'sparse_hessian.htm'
];
var list_current0 = [
'sparse_hessian.htm#Syntax',
'sparse_hessian.htm#Purpose',
'sparse_hessian.htm#f',
'sparse_hessian.htm#x',
'sparse_hessian.htm#w',
'sparse_hessian.htm#p',
'sparse_hessian.htm#p.Purpose',
'sparse_hessian.htm#p.work',
'sparse_hessian.htm#p.Column Subset',
'sparse_hessian.htm#row, col',
'sparse_hessian.htm#hes',
'sparse_hessian.htm#work',
'sparse_hessian.htm#work.color_method',
'sparse_hessian.htm#work.colpack.star Deprecated 2017-06-01',
'sparse_hessian.htm#work.p',
'sparse_hessian.htm#n_sweep',
'sparse_hessian.htm#BaseVector',
'sparse_hessian.htm#SetVector',
'sparse_hessian.htm#SetVector.Restrictions',
'sparse_hessian.htm#SizeVector',
'sparse_hessian.htm#Uses Forward',
'sparse_hessian.htm#Example',
'sparse_hessian.htm#Subset Hessian'
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