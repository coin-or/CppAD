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
'exp_eps.htm',
'exp_eps_rev1.htm'
];
var list_down3 = [
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
var list_down2 = [
'exp_2.htm',
'exp_eps.htm',
'exp_apx.cpp.htm'
];
var list_down1 = [
'exp_eps.hpp.htm',
'exp_eps.cpp.htm',
'exp_eps_for0.htm',
'exp_eps_for1.htm',
'exp_eps_rev1.htm',
'exp_eps_for2.htm',
'exp_eps_rev2.htm',
'exp_eps_cppad.htm'
];
var list_down0 = [
'exp_eps_rev1.cpp.htm'
];
var list_current0 = [
'exp_eps_rev1.htm#Purpose',
'exp_eps_rev1.htm#Mathematical Form',
'exp_eps_rev1.htm#epsilon',
'exp_eps_rev1.htm#f_7',
'exp_eps_rev1.htm#Index 7: f_6',
'exp_eps_rev1.htm#Index 6: f_5',
'exp_eps_rev1.htm#Index 5: f_4',
'exp_eps_rev1.htm#Index 4: f_3',
'exp_eps_rev1.htm#Index 3: f_2',
'exp_eps_rev1.htm#Index 2: f_1',
'exp_eps_rev1.htm#Verification',
'exp_eps_rev1.htm#Exercises'
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
