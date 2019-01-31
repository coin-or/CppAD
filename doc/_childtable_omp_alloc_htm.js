// Child table for section omp_alloc
document.write('\
<select onchange="omp_alloc_child(this)">\
<option>omp_alloc-&gt;</option>\
<option>omp_max_num_threads</option>\
<option>omp_in_parallel</option>\
<option>omp_get_thread_num</option>\
<option>omp_get_memory</option>\
<option>omp_return_memory</option>\
<option>omp_free_available</option>\
<option>omp_inuse</option>\
<option>omp_available</option>\
<option>omp_create_array</option>\
<option>omp_delete_array</option>\
<option>omp_efficient</option>\
<option>old_max_num_threads</option>\
</select>\
');
function omp_alloc_child(item)
{	var child_list = [
		'omp_max_num_threads.htm',
		'omp_in_parallel.htm',
		'omp_get_thread_num.htm',
		'omp_get_memory.htm',
		'omp_return_memory.htm',
		'omp_free_available.htm',
		'omp_inuse.htm',
		'omp_available.htm',
		'omp_create_array.htm',
		'omp_delete_array.htm',
		'omp_efficient.htm',
		'old_max_num_threads.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
