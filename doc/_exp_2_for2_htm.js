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
'exp_2_for2.htm'
];
var list_current0 = [
'exp_2_for2.htm#Second Order Expansion',
'exp_2_for2.htm#Purpose',
'exp_2_for2.htm#Mathematical Form',
'exp_2_for2.htm#Operation Sequence',
'exp_2_for2.htm#Operation Sequence.Index',
'exp_2_for2.htm#Operation Sequence.Zero',
'exp_2_for2.htm#Operation Sequence.Operation',
'exp_2_for2.htm#Operation Sequence.First',
'exp_2_for2.htm#Operation Sequence.Derivative',
'exp_2_for2.htm#Operation Sequence.Second',
'exp_2_for2.htm#Operation Sequence.Sweep',
'exp_2_for2.htm#Return Value',
'exp_2_for2.htm#Verification',
'exp_2_for2.htm#Exercises'
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
