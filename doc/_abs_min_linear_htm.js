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
'abs_min_linear.htm'
];
var list_current0 = [
'abs_min_linear.htm#Syntax',
'abs_min_linear.htm#Prototype',
'abs_min_linear.htm#Source',
'abs_min_linear.htm#Purpose',
'abs_min_linear.htm#DblVector',
'abs_min_linear.htm#SizeVector',
'abs_min_linear.htm#f',
'abs_min_linear.htm#level',
'abs_min_linear.htm#n',
'abs_min_linear.htm#m',
'abs_min_linear.htm#s',
'abs_min_linear.htm#g',
'abs_min_linear.htm#g_hat',
'abs_min_linear.htm#g_jac',
'abs_min_linear.htm#bound',
'abs_min_linear.htm#epsilon',
'abs_min_linear.htm#maxitr',
'abs_min_linear.htm#delta_x',
'abs_min_linear.htm#Method',
'abs_min_linear.htm#Method.sigma',
'abs_min_linear.htm#Method.Cutting Planes',
'abs_min_linear.htm#Method.Iteration',
'abs_min_linear.htm#Example'
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
