var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'introduction.htm',
'exp_eps.htm'
];
var list_down2 = [
'install.htm',
'introduction.htm',
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
'exp_2.htm',
'exp_eps.htm',
'exp_apx.cpp.htm'
];
var list_down0 = [
'exp_eps.hpp.htm',
'exp_eps.cpp.htm',
'exp_eps_for0.htm',
'exp_eps_for1.htm',
'exp_eps_rev1.htm',
'exp_eps_for2.htm',
'exp_eps_rev2.htm',
'exp_eps_cppad.htm'
];
var list_current0 = [
'exp_eps.htm#Syntax',
'exp_eps.htm#Purpose',
'exp_eps.htm#Mathematical Function',
'exp_eps.htm#include',
'exp_eps.htm#x',
'exp_eps.htm#epsilon',
'exp_eps.htm#y',
'exp_eps.htm#Type',
'exp_eps.htm#Implementation',
'exp_eps.htm#Test',
'exp_eps.htm#Exercises'
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
