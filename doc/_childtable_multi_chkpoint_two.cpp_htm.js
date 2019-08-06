// Child table for section multi_chkpoint_two.cpp
document.write('\
<select onchange="multi_chkpoint_two.cpp_child(this)">\
<option>multi_chkpoint_two.cpp-&gt;</option>\
<option>multi_chkpoint_two_algo</option>\
<option>multi_chkpoint_two_common</option>\
<option>multi_chkpoint_two_setup</option>\
<option>multi_chkpoint_two_worker</option>\
<option>multi_chkpoint_two_takedown</option>\
<option>multi_chkpoint_two_run</option>\
<option>multi_chkpoint_two_time</option>\
</select>\
');
function multi_chkpoint_two.cpp_child(item)
{	var child_list = [
		'multi_chkpoint_two_algo.htm',
		'multi_chkpoint_two_common.htm',
		'multi_chkpoint_two_setup.htm',
		'multi_chkpoint_two_worker.htm',
		'multi_chkpoint_two_takedown.htm',
		'multi_chkpoint_two_run.htm',
		'multi_chkpoint_two_time.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
