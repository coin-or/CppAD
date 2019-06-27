// Child table for section multi_atomic.cpp
document.write('\
<select onchange="multi_atomic__46__cpp_child(this)">\
<option>multi_atomic.cpp-&gt;</option>\
<option>multi_atomic_user</option>\
<option>multi_atomic_common</option>\
<option>multi_atomic_setup</option>\
<option>multi_atomic_worker</option>\
<option>multi_atomic_takedown</option>\
<option>multi_atomic_run</option>\
<option>multi_atomic_time</option>\
</select>\
');
function multi_atomic__46__cpp_child(item)
{	var child_list = [
		'multi_atomic_user.htm',
		'multi_atomic_common.htm',
		'multi_atomic_setup.htm',
		'multi_atomic_worker.htm',
		'multi_atomic_takedown.htm',
		'multi_atomic_run.htm',
		'multi_atomic_time.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
