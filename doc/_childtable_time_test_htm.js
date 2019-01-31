// Child table for section time_test
document.write('\
<select onchange="time_test_child(this)">\
<option>time_test-&gt;</option>\
<option>elapsed_seconds</option>\
<option>time_test.cpp</option>\
</select>\
');
function time_test_child(item)
{	var child_list = [
		'elapsed_seconds.htm',
		'time_test.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
