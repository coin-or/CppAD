var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'ipopt_solve.htm'
];
var list_current0 = [
'ipopt_solve.htm#Syntax',
'ipopt_solve.htm#Purpose',
'ipopt_solve.htm#Include File',
'ipopt_solve.htm#Bvector',
'ipopt_solve.htm#Dvector',
'ipopt_solve.htm#options',
'ipopt_solve.htm#options.Retape',
'ipopt_solve.htm#options.Sparse',
'ipopt_solve.htm#options.String',
'ipopt_solve.htm#options.Numeric',
'ipopt_solve.htm#options.Integer',
'ipopt_solve.htm#xi',
'ipopt_solve.htm#xl',
'ipopt_solve.htm#xu',
'ipopt_solve.htm#gl',
'ipopt_solve.htm#gu',
'ipopt_solve.htm#fg_eval',
'ipopt_solve.htm#fg_eval.ADvector',
'ipopt_solve.htm#fg_eval.x',
'ipopt_solve.htm#fg_eval.fg',
'ipopt_solve.htm#solution',
'ipopt_solve.htm#solution.status',
'ipopt_solve.htm#solution.x',
'ipopt_solve.htm#solution.zl',
'ipopt_solve.htm#solution.zu',
'ipopt_solve.htm#solution.g',
'ipopt_solve.htm#solution.lambda',
'ipopt_solve.htm#solution.obj_value',
'ipopt_solve.htm#Example',
'ipopt_solve.htm#Example.get_started',
'ipopt_solve.htm#Example.retape',
'ipopt_solve.htm#Example.ode_inverse'
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
