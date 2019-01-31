// Child table for section thread_alloc
document.write('\
<select onchange="thread_alloc_child(this)">\
<option>thread_alloc-&gt;</option>\
<option>thread_alloc.cpp</option>\
<option>ta_parallel_setup</option>\
<option>ta_num_threads</option>\
<option>ta_in_parallel</option>\
<option>ta_thread_num</option>\
<option>ta_get_memory</option>\
<option>ta_return_memory</option>\
<option>ta_free_available</option>\
<option>ta_hold_memory</option>\
<option>ta_inuse</option>\
<option>ta_available</option>\
<option>ta_create_array</option>\
<option>ta_delete_array</option>\
<option>ta_free_all</option>\
</select>\
');
function thread_alloc_child(item)
{	var child_list = [
		'thread_alloc.cpp.htm',
		'ta_parallel_setup.htm',
		'ta_num_threads.htm',
		'ta_in_parallel.htm',
		'ta_thread_num.htm',
		'ta_get_memory.htm',
		'ta_return_memory.htm',
		'ta_free_available.htm',
		'ta_hold_memory.htm',
		'ta_inuse.htm',
		'ta_available.htm',
		'ta_create_array.htm',
		'ta_delete_array.htm',
		'ta_free_all.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
