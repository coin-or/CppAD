// Child table for section multi_newton.cpp
document.write('\
<select onchange="multi_newton.cpp_child(this)">\
<option>multi_newton.cpp-&gt;</option>\
<option>multi_newton_common</option>\
<option>multi_newton_setup</option>\
<option>multi_newton_worker</option>\
<option>multi_newton_takedown</option>\
<option>multi_newton_run</option>\
<option>multi_newton_time</option>\
</select>\
');
function multi_newton.cpp_child(item)
{	var child_list = [
		'multi_newton_common.htm',
		'multi_newton_setup.htm',
		'multi_newton_worker.htm',
		'multi_newton_takedown.htm',
		'multi_newton_run.htm',
		'multi_newton_time.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
