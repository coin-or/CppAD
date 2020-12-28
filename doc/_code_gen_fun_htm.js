var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'example.htm',
'general.htm',
'code_gen_fun.htm'
];
var list_current0 = [
'code_gen_fun.htm#Syntax',
'code_gen_fun.htm#Syntax.Constructors',
'code_gen_fun.htm#Syntax.swap',
'code_gen_fun.htm#Syntax.function',
'code_gen_fun.htm#Syntax.jacobian',
'code_gen_fun.htm#Syntax.sparse_jacobian',
'code_gen_fun.htm#Prototype',
'code_gen_fun.htm#Prototype.Constructors',
'code_gen_fun.htm#Prototype.Operations',
'code_gen_fun.htm#CppAD::cg::CG&lt;double&gt;',
'code_gen_fun.htm#Speed',
'code_gen_fun.htm#fun_name',
'code_gen_fun.htm#other_fun',
'code_gen_fun.htm#file_name',
'code_gen_fun.htm#cg_fun',
'code_gen_fun.htm#eval_jac',
'code_gen_fun.htm#swap',
'code_gen_fun.htm#x',
'code_gen_fun.htm#y',
'code_gen_fun.htm#jacobian',
'code_gen_fun.htm#jacobian.J',
'code_gen_fun.htm#jacobian.Speed',
'code_gen_fun.htm#sparse_jacobian',
'code_gen_fun.htm#Jrcv',
'code_gen_fun.htm#Jrcv.Speed',
'code_gen_fun.htm#Examples',
'code_gen_fun.htm#Implementation'
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
