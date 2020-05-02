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
'cppad_vector.htm'
];
var list_current0 = [
'cppad_vector.htm#Syntax',
'cppad_vector.htm#Description',
'cppad_vector.htm#Include',
'cppad_vector.htm#Include.Deprecated 2019-08-19',
'cppad_vector.htm#Integer Size',
'cppad_vector.htm#capacity',
'cppad_vector.htm#swap',
'cppad_vector.htm#Assignment',
'cppad_vector.htm#Assignment.Check Size',
'cppad_vector.htm#Assignment.Return Reference',
'cppad_vector.htm#Assignment.Move Semantics',
'cppad_vector.htm#Element Access',
'cppad_vector.htm#Element Access.i',
'cppad_vector.htm#Element Access.Error Checking',
'cppad_vector.htm#push_back',
'cppad_vector.htm#push_vector',
'cppad_vector.htm#Output',
'cppad_vector.htm#resize',
'cppad_vector.htm#clear',
'cppad_vector.htm#data',
'cppad_vector.htm#Iterators',
'cppad_vector.htm#Iterators.Syntax',
'cppad_vector.htm#Iterators.iterator',
'cppad_vector.htm#Iterators.const_iterator',
'cppad_vector.htm#Iterators.begin',
'cppad_vector.htm#Iterators.end',
'cppad_vector.htm#Iterators.Error Checking',
'cppad_vector.htm#vectorBool',
'cppad_vector.htm#vectorBool.Memory',
'cppad_vector.htm#vectorBool.bit_per_unit',
'cppad_vector.htm#vectorBool.data',
'cppad_vector.htm#vectorBool.Iterators',
'cppad_vector.htm#vectorBool.Output',
'cppad_vector.htm#vectorBool.Element Type',
'cppad_vector.htm#Memory and Parallel Mode',
'cppad_vector.htm#Example',
'cppad_vector.htm#Exercise'
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
