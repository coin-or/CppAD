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
'forward.htm',
'forward_order.htm'
];
var list_current0 = [
'forward_order.htm#Syntax',
'forward_order.htm#Purpose',
'forward_order.htm#Purpose.Function Values',
'forward_order.htm#Purpose.Derivative Values',
'forward_order.htm#Notation',
'forward_order.htm#Notation.n',
'forward_order.htm#Notation.m',
'forward_order.htm#f',
'forward_order.htm#One Order',
'forward_order.htm#q',
'forward_order.htm#xq',
'forward_order.htm#xq.One Order',
'forward_order.htm#xq.Multiple Orders',
'forward_order.htm#xq.Restrictions',
'forward_order.htm#s',
'forward_order.htm#X(t)',
'forward_order.htm#Y(t)',
'forward_order.htm#yq',
'forward_order.htm#yq.One Order',
'forward_order.htm#yq.Multiple Orders',
'forward_order.htm#BaseVector',
'forward_order.htm#Zero Order',
'forward_order.htm#First Order',
'forward_order.htm#Second Order',
'forward_order.htm#Example'
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
