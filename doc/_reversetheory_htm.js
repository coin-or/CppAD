var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'theory.htm',
'reversetheory.htm'
];
var list_down2 = [
'install.htm',
'theory.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down1 = [
'introduction.htm',
'forwardtheory.htm',
'reversetheory.htm',
'reverse_identity.htm',
'taylor_ode.htm',
'research.htm'
];
var list_down0 = [
'exp_reverse.htm',
'log_reverse.htm',
'sqrt_reverse.htm',
'sin_cos_reverse.htm',
'atan_reverse.htm',
'asin_reverse.htm',
'acos_reverse.htm',
'tan_reverse.htm',
'erf_reverse.htm'
];
var list_current0 = [
'reversetheory.htm#Taylor Notation',
'reversetheory.htm#Binary Operators',
'reversetheory.htm#Binary Operators.Addition',
'reversetheory.htm#Binary Operators.Subtraction',
'reversetheory.htm#Binary Operators.Multiplication',
'reversetheory.htm#Binary Operators.Division',
'reversetheory.htm#Standard Math Functions',
'reversetheory.htm#Contents'
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
