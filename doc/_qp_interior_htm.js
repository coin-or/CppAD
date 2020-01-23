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
'qp_interior.htm'
];
var list_current0 = [
'qp_interior.htm#Syntax',
'qp_interior.htm#Prototype',
'qp_interior.htm#Source',
'qp_interior.htm#Purpose',
'qp_interior.htm#Problem',
'qp_interior.htm#Vector',
'qp_interior.htm#level',
'qp_interior.htm#c',
'qp_interior.htm#C',
'qp_interior.htm#g',
'qp_interior.htm#G',
'qp_interior.htm#epsilon',
'qp_interior.htm#maxitr',
'qp_interior.htm#xin',
'qp_interior.htm#xout',
'qp_interior.htm#yout',
'qp_interior.htm#sout',
'qp_interior.htm#ok',
'qp_interior.htm#KKT Conditions',
'qp_interior.htm#Newton Step',
'qp_interior.htm#Newton Step.Elementary Row Reduction',
'qp_interior.htm#Solution',
'qp_interior.htm#Example'
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
