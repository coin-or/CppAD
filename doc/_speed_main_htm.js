var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'speed.htm',
'speed_main.htm'
];
var list_current0 = [
'speed_main.htm#Syntax',
'speed_main.htm#Purpose',
'speed_main.htm#package',
'speed_main.htm#package.AD Package',
'speed_main.htm#package.double',
'speed_main.htm#package.profile',
'speed_main.htm#test',
'speed_main.htm#test.correct',
'speed_main.htm#test.speed',
'speed_main.htm#seed',
'speed_main.htm#Global Options',
'speed_main.htm#Global Options.onetape',
'speed_main.htm#Global Options.memory',
'speed_main.htm#Global Options.optimize',
'speed_main.htm#Global Options.atomic',
'speed_main.htm#Global Options.hes2jac',
'speed_main.htm#Global Options.subgraph',
'speed_main.htm#Sparsity Options',
'speed_main.htm#Sparsity Options.boolsparsity',
'speed_main.htm#Sparsity Options.revsparsity',
'speed_main.htm#Sparsity Options.subsparsity',
'speed_main.htm#Sparsity Options.colpack',
'speed_main.htm#Sparsity Options.symmetric',
'speed_main.htm#Correctness Results',
'speed_main.htm#Speed Results',
'speed_main.htm#Speed Results.n_color',
'speed_main.htm#Link Routines'
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
