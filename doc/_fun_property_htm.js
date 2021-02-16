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
'record_adfun.htm',
'fun_property.htm'
];
var list_current0 = [
'fun_property.htm#Syntax',
'fun_property.htm#Syntax.See Also',
'fun_property.htm#Purpose',
'fun_property.htm#f',
'fun_property.htm#Domain',
'fun_property.htm#Range',
'fun_property.htm#Parameter',
'fun_property.htm#size_var',
'fun_property.htm#size_par',
'fun_property.htm#size_op',
'fun_property.htm#size_op_arg',
'fun_property.htm#size_text',
'fun_property.htm#size_VecAD',
'fun_property.htm#size_random',
'fun_property.htm#size_dyn_ind',
'fun_property.htm#size_dyn_par',
'fun_property.htm#size_dyn_arg',
'fun_property.htm#size_op_seq',
'fun_property.htm#Example'
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
