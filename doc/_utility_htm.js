var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'utility.htm'
];
var list_current0 = [
'utility.htm#Testing',
'utility.htm#C++ Concepts',
'utility.htm#General Numerical Routines',
'utility.htm#Miscellaneous',
'utility.htm#Miscellaneous.Error Handler',
'utility.htm#Miscellaneous.The CppAD Vector Template Class',
'utility.htm#Miscellaneous.Multi-Threading Memory Allocation',
'utility.htm#Miscellaneous.Sorting Indices',
'utility.htm#Miscellaneous.to_string',
'utility.htm#Miscellaneous.set_union',
'utility.htm#Miscellaneous.Sparse Matrices',
'utility.htm#Miscellaneous.Dynamic Libraries'
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
