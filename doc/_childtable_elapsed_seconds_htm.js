// Child table for section elapsed_seconds
document.write('\
<select onchange="elapsed_seconds_child(this)">\
<option>elapsed_seconds-&gt;</option>\
<option>elapsed_seconds.cpp</option>\
</select>\
');
function elapsed_seconds_child(item)
{	var child_list = [
		'elapsed_seconds.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
