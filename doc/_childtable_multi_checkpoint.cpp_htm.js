// Child table for section multi_checkpoint.cpp
document.write('\
<select onchange="multi_checkpoint__46__cpp_child(this)">\
<option>multi_checkpoint.cpp-&gt;</option>\
<option>multi_checkpoint_algo</option>\
<option>multi_checkpoint_common</option>\
<option>multi_checkpoint_setup</option>\
<option>multi_checkpoint_worker</option>\
<option>multi_checkpoint_takedown</option>\
<option>multi_checkpoint_run</option>\
<option>multi_checkpoint_time</option>\
</select>\
');
function multi_checkpoint__46__cpp_child(item)
{	var child_list = [
		'multi_checkpoint_algo.htm',
		'multi_checkpoint_common.htm',
		'multi_checkpoint_setup.htm',
		'multi_checkpoint_worker.htm',
		'multi_checkpoint_takedown.htm',
		'multi_checkpoint_run.htm',
		'multi_checkpoint_time.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
