var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'multi_thread.htm',
'thread_test.cpp.htm',
'harmonic.cpp.htm',
'harmonic_sum.htm'
];
var list_down3 = [
'parallel_ad.htm',
'thread_test.cpp.htm'
];
var list_down2 = [
'a11c_openmp.cpp.htm',
'a11c_bthread.cpp.htm',
'a11c_pthread.cpp.htm',
'simple_ad_openmp.cpp.htm',
'simple_ad_bthread.cpp.htm',
'simple_ad_pthread.cpp.htm',
'team_example.cpp.htm',
'harmonic.cpp.htm',
'multi_atomic.cpp.htm',
'multi_checkpoint.cpp.htm',
'multi_newton.cpp.htm',
'team_thread.hpp.htm'
];
var list_down1 = [
'harmonic_common.htm',
'harmonic_setup.htm',
'harmonic_worker.htm',
'harmonic_takedown.htm',
'harmonic_sum.htm',
'harmonic_time.htm'
];
var list_current0 = [
'harmonic_sum.htm#Syntax',
'harmonic_sum.htm#Purpose',
'harmonic_sum.htm#Thread',
'harmonic_sum.htm#ok',
'harmonic_sum.htm#sum',
'harmonic_sum.htm#num_sum',
'harmonic_sum.htm#Source'
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
