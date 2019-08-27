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
'json_op_define.htm#Unary Operators',
'json_op_define.htm#Unary Operators.abs',
'json_op_define.htm#Unary Operators.acos',
'json_op_define.htm#Unary Operators.asin',
'json_op_define.htm#Unary Operators.atan',
'json_op_define.htm#Unary Operators.cosh',
'json_op_define.htm#Unary Operators.cos',
'json_op_define.htm#Unary Operators.exp',
'json_op_define.htm#Unary Operators.log',
'json_op_define.htm#Unary Operators.sign',
'json_op_define.htm#Unary Operators.sinh',
'json_op_define.htm#Unary Operators.sin',
'json_op_define.htm#Unary Operators.sqrt',
'json_op_define.htm#Unary Operators.tanh',
'json_op_define.htm#Unary Operators.tan',
'json_op_define.htm#Unary Operators.C++11',
'json_op_define.htm#Unary Operators.asinh',
'json_op_define.htm#Unary Operators.atanh',
'json_op_define.htm#Unary Operators.erf',
'json_op_define.htm#Unary Operators.erfc',
'json_op_define.htm#Unary Operators.expm1',
'json_op_define.htm#Unary Operators.log1p',
'json_op_define.htm#Unary Operators.acosh',
'json_op_define.htm#Binary Operators',
'json_op_define.htm#Binary Operators.add',
'json_op_define.htm#Binary Operators.div',
'json_op_define.htm#Binary Operators.mul',
'json_op_define.htm#Binary Operators.sub',
'json_op_define.htm#atom',
'json_op_define.htm#atom.Example',
'json_op_define.htm#atom.call_name',
'json_op_define.htm#atom.n_result',
'json_op_define.htm#atom.n_arg',
'json_op_define.htm#atom.first_arg, ..., last_arg',
'json_op_define.htm#sum',
'json_op_define.htm#sum.Example'
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
