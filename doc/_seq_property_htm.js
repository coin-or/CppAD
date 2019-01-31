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
'seq_property.htm'
];
var list_current0 = [
'seq_property.htm#Syntax',
'seq_property.htm#Syntax.See Also',
'seq_property.htm#Purpose',
'seq_property.htm#f',
'seq_property.htm#Domain',
'seq_property.htm#Range',
'seq_property.htm#Parameter',
'seq_property.htm#size_var',
'seq_property.htm#size_par',
'seq_property.htm#size_op',
'seq_property.htm#size_op_arg',
'seq_property.htm#size_text',
'seq_property.htm#size_VecAD',
'seq_property.htm#size_random',
'seq_property.htm#size_dyn_ind',
'seq_property.htm#size_dyn_par',
'seq_property.htm#size_dyn_arg',
'seq_property.htm#size_op_seq',
'seq_property.htm#Example'
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
