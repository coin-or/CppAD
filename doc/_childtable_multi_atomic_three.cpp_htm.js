// Child table for section multi_atomic_three.cpp
document.write('\
<select onchange="multi_atomic_three.cpp_child(this)">\
<option>multi_atomic_three.cpp-&gt;</option>\
<option>multi_atomic_three_user</option>\
<option>multi_atomic_three_common</option>\
<option>multi_atomic_three_setup</option>\
<option>multi_atomic_three_worker</option>\
<option>multi_atomic_three_takedown</option>\
<option>multi_atomic_three_run</option>\
<option>multi_atomic_three_time</option>\
</select>\
');
function multi_atomic_three.cpp_child(item)
{	var child_list = [
		'multi_atomic_three_user.htm',
		'multi_atomic_three_common.htm',
		'multi_atomic_three_setup.htm',
		'multi_atomic_three_worker.htm',
		'multi_atomic_three_takedown.htm',
		'multi_atomic_three_run.htm',
		'multi_atomic_three_time.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
