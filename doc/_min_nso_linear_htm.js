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
'other_adfun.htm',
'abs_normal_fun.htm',
'example_abs_normal.htm',
'min_nso_linear.htm'
];
var list_current0 = [
'min_nso_linear.htm#Syntax',
'min_nso_linear.htm#Prototype',
'min_nso_linear.htm#Source',
'min_nso_linear.htm#Purpose',
'min_nso_linear.htm#DblVector',
'min_nso_linear.htm#SizeVector',
'min_nso_linear.htm#f',
'min_nso_linear.htm#f.n',
'min_nso_linear.htm#f.m',
'min_nso_linear.htm#f.s',
'min_nso_linear.htm#level',
'min_nso_linear.htm#g',
'min_nso_linear.htm#a',
'min_nso_linear.htm#epsilon',
'min_nso_linear.htm#maxitr',
'min_nso_linear.htm#b_in',
'min_nso_linear.htm#x_in',
'min_nso_linear.htm#x_out',
'min_nso_linear.htm#Example'
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
