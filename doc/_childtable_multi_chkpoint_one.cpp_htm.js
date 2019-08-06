// Child table for section multi_chkpoint_one.cpp
document.write('\
<select onchange="multi_chkpoint_one.cpp_child(this)">\
<option>multi_chkpoint_one.cpp-&gt;</option>\
<option>multi_chkpoint_one_algo</option>\
<option>multi_chkpoint_one_common</option>\
<option>multi_chkpoint_one_setup</option>\
<option>multi_chkpoint_one_worker</option>\
<option>multi_chkpoint_one_takedown</option>\
<option>multi_chkpoint_one_run</option>\
<option>multi_chkpoint_one_time</option>\
</select>\
');
function multi_chkpoint_one.cpp_child(item)
{	var child_list = [
		'multi_chkpoint_one_algo.htm',
		'multi_chkpoint_one_common.htm',
		'multi_chkpoint_one_setup.htm',
		'multi_chkpoint_one_worker.htm',
		'multi_chkpoint_one_takedown.htm',
		'multi_chkpoint_one_run.htm',
		'multi_chkpoint_one_time.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
