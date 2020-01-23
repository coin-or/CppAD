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
'qp_box.htm'
];
var list_current0 = [
'qp_box.htm#Syntax',
'qp_box.htm#Prototype',
'qp_box.htm#Source',
'qp_box.htm#Purpose',
'qp_box.htm#Problem',
'qp_box.htm#Vector',
'qp_box.htm#level',
'qp_box.htm#a',
'qp_box.htm#b',
'qp_box.htm#c',
'qp_box.htm#C',
'qp_box.htm#g',
'qp_box.htm#G',
'qp_box.htm#epsilon',
'qp_box.htm#maxitr',
'qp_box.htm#xin',
'qp_box.htm#xout',
'qp_box.htm#ok',
'qp_box.htm#KKT Conditions',
'qp_box.htm#Example'
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
