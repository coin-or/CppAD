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
'exp_eps.htm',
'exp_eps_rev2.htm'
];
var list_current0 = [
'exp_eps_rev2.htm#Purpose',
'exp_eps_rev2.htm#Mathematical Form',
'exp_eps_rev2.htm#epsilon',
'exp_eps_rev2.htm#f_7',
'exp_eps_rev2.htm#Index 7: f_6',
'exp_eps_rev2.htm#Index 6: f_5',
'exp_eps_rev2.htm#Index 5: f_4',
'exp_eps_rev2.htm#Index 4: f_3',
'exp_eps_rev2.htm#Index 3: f_2',
'exp_eps_rev2.htm#Index 2: f_1',
'exp_eps_rev2.htm#Verification',
'exp_eps_rev2.htm#Exercises'
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
