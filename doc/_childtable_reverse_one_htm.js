// Child table for section reverse_one
document.write('\
<select onchange="reverse_one_child(this)">\
<option>reverse_one-&gt;</option>\
<option>reverse_one.cpp</option>\
</select>\
');
function reverse_one_child(item)
{	var child_list = [
		'reverse_one.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
