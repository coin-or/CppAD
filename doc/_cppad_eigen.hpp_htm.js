var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'example.htm',
'general.htm',
'cppad_eigen.hpp.htm'
];
var list_current0 = [
'cppad_eigen.hpp.htm#Syntax',
'cppad_eigen.hpp.htm#Purpose',
'cppad_eigen.hpp.htm#Example',
'cppad_eigen.hpp.htm#CppAD Declarations',
'cppad_eigen.hpp.htm#std Declarations',
'cppad_eigen.hpp.htm#Include Eigen/Core',
'cppad_eigen.hpp.htm#Eigen NumTraits',
'cppad_eigen.hpp.htm#Eigen ScalarBinaryOpTraits',
'cppad_eigen.hpp.htm#CppAD Namespace',
'cppad_eigen.hpp.htm#eigen_vector',
'cppad_eigen.hpp.htm#Include cppad.hpp',
'cppad_eigen.hpp.htm#std Definitions'
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
