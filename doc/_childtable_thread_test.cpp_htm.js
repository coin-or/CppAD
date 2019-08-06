// Child table for section thread_test.cpp
document.write('\
<select onchange="thread_test.cpp_child(this)">\
<option>thread_test.cpp-&gt;</option>\
<option>a11c_openmp.cpp</option>\
<option>a11c_bthread.cpp</option>\
<option>a11c_pthread.cpp</option>\
<option>simple_ad_openmp.cpp</option>\
<option>simple_ad_bthread.cpp</option>\
<option>simple_ad_pthread.cpp</option>\
<option>team_example.cpp</option>\
<option>harmonic.cpp</option>\
<option>multi_atomic_three.cpp</option>\
<option>multi_chkpoint_two.cpp</option>\
<option>multi_newton.cpp</option>\
<option>team_thread.hpp</option>\
</select>\
');
function thread_test.cpp_child(item)
{	var child_list = [
		'a11c_openmp.cpp.htm',
		'a11c_bthread.cpp.htm',
		'a11c_pthread.cpp.htm',
		'simple_ad_openmp.cpp.htm',
		'simple_ad_bthread.cpp.htm',
		'simple_ad_pthread.cpp.htm',
		'team_example.cpp.htm',
		'harmonic.cpp.htm',
		'multi_atomic_three.cpp.htm',
		'multi_chkpoint_two.cpp.htm',
		'multi_newton.cpp.htm',
		'team_thread.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
