var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'utility.htm',
'odegearcontrol.htm'
];
var list_current0 = [
'odegearcontrol.htm#Syntax',
'odegearcontrol.htm#Purpose',
'odegearcontrol.htm#Include',
'odegearcontrol.htm#Notation',
'odegearcontrol.htm#xf',
'odegearcontrol.htm#Fun',
'odegearcontrol.htm#Fun.t',
'odegearcontrol.htm#Fun.x',
'odegearcontrol.htm#Fun.f',
'odegearcontrol.htm#Fun.f_x',
'odegearcontrol.htm#Fun.Warning',
'odegearcontrol.htm#M',
'odegearcontrol.htm#ti',
'odegearcontrol.htm#tf',
'odegearcontrol.htm#xi',
'odegearcontrol.htm#smin',
'odegearcontrol.htm#smax',
'odegearcontrol.htm#sini',
'odegearcontrol.htm#eabs',
'odegearcontrol.htm#erel',
'odegearcontrol.htm#ef',
'odegearcontrol.htm#maxabs',
'odegearcontrol.htm#nstep',
'odegearcontrol.htm#Error Criteria Discussion',
'odegearcontrol.htm#Scalar',
'odegearcontrol.htm#Vector',
'odegearcontrol.htm#Example',
'odegearcontrol.htm#Theory',
'odegearcontrol.htm#Source Code'
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
