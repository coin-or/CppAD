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
'compiled_fun.htm'
];
var list_current0 = [
'compiled_fun.htm#Syntax',
'compiled_fun.htm#Syntax.Constructor',
'compiled_fun.htm#Syntax.Operations',
'compiled_fun.htm#Prototype',
'compiled_fun.htm#Prototype.Constructors',
'compiled_fun.htm#Prototype.Operations',
'compiled_fun.htm#CppAD::cg::CG&lt;double&gt;',
'compiled_fun.htm#Speed',
'compiled_fun.htm#fun_name',
'compiled_fun.htm#other_fun',
'compiled_fun.htm#file_name',
'compiled_fun.htm#cg_fun',
'compiled_fun.htm#eval_jac',
'compiled_fun.htm#swap',
'compiled_fun.htm#x',
'compiled_fun.htm#y',
'compiled_fun.htm#J',
'compiled_fun.htm#Jrcv',
'compiled_fun.htm#Examples'
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
