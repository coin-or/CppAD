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
'deprecated.htm',
'fundeprecated.htm'
];
var list_current0 = [
'fundeprecated.htm#Syntax',
'fundeprecated.htm#Purpose',
'fundeprecated.htm#Dependent',
'fundeprecated.htm#Dependent.Deprecated 2007-08-07',
'fundeprecated.htm#Order',
'fundeprecated.htm#Order.Deprecated 2006-03-31',
'fundeprecated.htm#Memory',
'fundeprecated.htm#Memory.Deprecated 2006-03-31',
'fundeprecated.htm#Size',
'fundeprecated.htm#Size.Deprecated 2006-04-03',
'fundeprecated.htm#taylor_size',
'fundeprecated.htm#taylor_size.Deprecated 2006-06-17',
'fundeprecated.htm#use_VecAD',
'fundeprecated.htm#use_VecAD.Deprecated 2006-04-08',
'fundeprecated.htm#size_taylor',
'fundeprecated.htm#size_taylor.Deprecated 2014-03-18',
'fundeprecated.htm#capacity_taylor',
'fundeprecated.htm#capacity_taylor.Deprecated 2014-03-18'
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
