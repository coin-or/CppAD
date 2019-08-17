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
'json_ad_graph.htm',
'json_op_define.htm'
];
var list_current0 = [
'json_op_define.htm#Notation',
'json_op_define.htm#Notation.op_code',
'json_op_define.htm#Notation.Arguments',
'json_op_define.htm#add',
'json_op_define.htm#add.Example',
'json_op_define.htm#atom',
'json_op_define.htm#atom.Example',
'json_op_define.htm#div',
'json_op_define.htm#div.Example',
'json_op_define.htm#div.call_name',
'json_op_define.htm#div.n_result',
'json_op_define.htm#div.n_arg',
'json_op_define.htm#div.first_arg, ..., last_arg',
'json_op_define.htm#mul',
'json_op_define.htm#mul.Example',
'json_op_define.htm#sub',
'json_op_define.htm#sub.Example',
'json_op_define.htm#sum',
'json_op_define.htm#sum.Example',
'json_op_define.htm#Contents'
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
