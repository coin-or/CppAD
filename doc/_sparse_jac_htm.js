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
'sparse_jac.htm'
];
var list_current0 = [
'sparse_jac.htm#Syntax',
'sparse_jac.htm#Purpose',
'sparse_jac.htm#SizeVector',
'sparse_jac.htm#BaseVector',
'sparse_jac.htm#sparse_jac_for',
'sparse_jac.htm#sparse_jac_rev',
'sparse_jac.htm#f',
'sparse_jac.htm#group_max',
'sparse_jac.htm#x',
'sparse_jac.htm#subset',
'sparse_jac.htm#pattern',
'sparse_jac.htm#coloring',
'sparse_jac.htm#coloring.cppad',
'sparse_jac.htm#coloring.colpack',
'sparse_jac.htm#work',
'sparse_jac.htm#n_color',
'sparse_jac.htm#Uses Forward',
'sparse_jac.htm#Example'
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
