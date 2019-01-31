// Child table for section speed_test
document.write('\
<select onchange="speed_test_child(this)">\
<option>speed_test-&gt;</option>\
<option>speed_test.cpp</option>\
</select>\
');
function speed_test_child(item)
{	var child_list = [
		'speed_test.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
