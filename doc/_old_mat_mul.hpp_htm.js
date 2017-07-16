var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'appendix.htm',
'deprecated.htm',
'old_atomic.htm',
'old_mat_mul.cpp.htm',
'old_mat_mul.hpp.htm'
];
var list_down3 = [
'include_deprecated.htm',
'fundeprecated.htm',
'comparechange.htm',
'omp_max_thread.htm',
'tracknewdel.htm',
'omp_alloc.htm',
'memory_leak.htm',
'epsilon.htm',
'test_vector.htm',
'cppad_ipopt_nlp.htm',
'old_atomic.htm',
'zdouble.htm',
'autotools.htm'
];
var list_down2 = [
'old_reciprocal.cpp.htm',
'old_usead_1.cpp.htm',
'old_usead_2.cpp.htm',
'old_tan.cpp.htm',
'old_mat_mul.cpp.htm'
];
var list_down1 = [
'old_mat_mul.hpp.htm'
];
var list_current0 = [
'old_mat_mul.hpp.htm#Syntax',
'old_mat_mul.hpp.htm#Example',
'old_mat_mul.hpp.htm#Begin Source',
'old_mat_mul.hpp.htm#Extra Call Information',
'old_mat_mul.hpp.htm#Matrix Indexing',
'old_mat_mul.hpp.htm#One Matrix Multiply',
'old_mat_mul.hpp.htm#Reverse Partials One Order',
'old_mat_mul.hpp.htm#Set Union',
'old_mat_mul.hpp.htm#CppAD User Atomic Callback Functions',
'old_mat_mul.hpp.htm#Declare mat_mul Function'
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
