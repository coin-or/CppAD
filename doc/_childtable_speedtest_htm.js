// Child table for section speedtest
document.write('\
<select onchange="speedtest_child(this)">\
<option>speedtest-&gt;</option>\
<option>speed_program.cpp</option>\
</select>\
');
function speedtest_child(item)
{	var child_list = [
		'speed_program.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
