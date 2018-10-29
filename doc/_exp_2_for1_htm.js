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
'introduction.htm',
'exp_2.htm',
'exp_2_for1.htm'
];
var list_down3 = [
'introduction.htm',
'forwardtheory.htm',
'reversetheory.htm',
'reverse_identity.htm',
'taylor_ode.htm',
'research.htm'
];
var list_down2 = [
'exp_2.htm',
'exp_eps.htm',
'exp_apx.cpp.htm'
];
var list_down1 = [
'exp_2.hpp.htm',
'exp_2.cpp.htm',
'exp_2_for0.htm',
'exp_2_for1.htm',
'exp_2_rev1.htm',
'exp_2_for2.htm',
'exp_2_rev2.htm',
'exp_2_cppad.htm'
];
var list_down0 = [
'exp_2_for1.cpp.htm'
];
var list_current0 = [
'exp_2_for1.htm#First Order Expansion',
'exp_2_for1.htm#Purpose',
'exp_2_for1.htm#Mathematical Form',
'exp_2_for1.htm#Operation Sequence',
'exp_2_for1.htm#Operation Sequence.Index',
'exp_2_for1.htm#Operation Sequence.Operation',
'exp_2_for1.htm#Operation Sequence.Zero Order',
'exp_2_for1.htm#Operation Sequence.Derivative',
'exp_2_for1.htm#Operation Sequence.First Order',
'exp_2_for1.htm#Operation Sequence.Sweep',
'exp_2_for1.htm#Return Value',
'exp_2_for1.htm#Verification',
'exp_2_for1.htm#Exercises'
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
