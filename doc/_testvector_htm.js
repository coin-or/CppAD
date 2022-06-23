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
'testvector.htm'
];
var list_current0 = [
'testvector.htm#Syntax',
'testvector.htm#Choice',
'testvector.htm#CppAD::vector',
'testvector.htm#CppAD::vector.CPPAD_CPPADVECTOR, Deprecated 2022-06-22',
'testvector.htm#std::vector',
'testvector.htm#std::vector.CPPAD_STDVECTOR, Deprecated 2022-06-22',
'testvector.htm#boost::numeric::ublas::vector',
'testvector.htm#boost::numeric::ublas::vector.CPPAD_BOOSTVECTOR, Deprecated 2022-06-22',
'testvector.htm#CppAD::eigen_vector',
'testvector.htm#CppAD::eigen_vector.CPPAD_EIGENVECTOR, Deprecated 2022-06-22'
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
