// Child table for section multi_thread
document.write('\
<select onchange="multi_thread_child(this)">\
<option>multi_thread-&gt;</option>\
<option>parallel_ad</option>\
<option>thread_test.cpp</option>\
</select>\
');
function multi_thread_child(item)
{	var child_list = [
		'parallel_ad.htm',
		'thread_test.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
