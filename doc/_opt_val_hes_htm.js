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
'numeric_ad.htm',
'opt_val_hes.htm'
];
var list_current0 = [
'opt_val_hes.htm#Syntax',
'opt_val_hes.htm#See Also',
'opt_val_hes.htm#Reference',
'opt_val_hes.htm#Purpose',
'opt_val_hes.htm#BaseVector',
'opt_val_hes.htm#x',
'opt_val_hes.htm#y',
'opt_val_hes.htm#Fun',
'opt_val_hes.htm#Fun.Fun::ad_vector',
'opt_val_hes.htm#Fun.fun.ell',
'opt_val_hes.htm#Fun.fun.s',
'opt_val_hes.htm#Fun.fun.sy',
'opt_val_hes.htm#jac',
'opt_val_hes.htm#hes',
'opt_val_hes.htm#signdet',
'opt_val_hes.htm#Example'
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
