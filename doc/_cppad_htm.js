var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm'
];
var list_current0 = [
'cppad.htm#Old Documentation',
'cppad.htm#Current Documentation',
'cppad.htm#Algorithmic Differentiation',
'cppad.htm#Features',
'cppad.htm#Features.Operator Overloading',
'cppad.htm#Features.Base Type',
'cppad.htm#Features.Recording Operations',
'cppad.htm#Features.Multi-Threading',
'cppad.htm#Features.Optimizing Operations Sequences',
'cppad.htm#Features.Dynamic Parameters',
'cppad.htm#Features.Derivative Calculations',
'cppad.htm#Features.Sparsity',
'cppad.htm#Features.Recording Derivative Operations',
'cppad.htm#Features.Atomic Functions',
'cppad.htm#Features.Logical Comparisons',
'cppad.htm#Features.Vectors',
'cppad.htm#Software Engineering',
'cppad.htm#Testing',
'cppad.htm#Testing.Correctness',
'cppad.htm#Testing.Speed',
'cppad.htm#Utilities',
'cppad.htm#Release Notes',
'cppad.htm#Example'
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
