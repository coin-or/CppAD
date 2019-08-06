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
'thread_test.cpp.htm'
];
var list_current0 = [
'thread_test.cpp.htm#Purpose',
'thread_test.cpp.htm#build',
'thread_test.cpp.htm#threading',
'thread_test.cpp.htm#program',
'thread_test.cpp.htm#Running Tests',
'thread_test.cpp.htm#a11c',
'thread_test.cpp.htm#simple_ad',
'thread_test.cpp.htm#team_example',
'thread_test.cpp.htm#test_time',
'thread_test.cpp.htm#test_time.max_threads',
'thread_test.cpp.htm#harmonic',
'thread_test.cpp.htm#harmonic.mega_sum',
'thread_test.cpp.htm#Atomic and Checkpoint',
'thread_test.cpp.htm#Atomic and Checkpoint.num_solve',
'thread_test.cpp.htm#multi_newton',
'thread_test.cpp.htm#multi_newton.num_zero',
'thread_test.cpp.htm#multi_newton.num_sub',
'thread_test.cpp.htm#multi_newton.num_sum',
'thread_test.cpp.htm#multi_newton.use_ad',
'thread_test.cpp.htm#Team Implementations',
'thread_test.cpp.htm#Source'
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
