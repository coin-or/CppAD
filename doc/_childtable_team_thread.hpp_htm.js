// Child table for section team_thread.hpp
document.write('\
<select onchange="team_thread.hpp_child(this)">\
<option>team_thread.hpp-&gt;</option>\
<option>team_openmp.cpp</option>\
<option>team_bthread.cpp</option>\
<option>team_pthread.cpp</option>\
</select>\
');
function team_thread.hpp_child(item)
{	var child_list = [
		'team_openmp.cpp.htm',
		'team_bthread.cpp.htm',
		'team_pthread.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
