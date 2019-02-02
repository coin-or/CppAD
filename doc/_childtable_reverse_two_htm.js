// Child table for section reverse_two
document.write('\
<select onchange="reverse_two_child(this)">\
<option>reverse_two-&gt;</option>\
<option>reverse_two.cpp</option>\
<option>hes_times_dir.cpp</option>\
</select>\
');
function reverse_two_child(item)
{	var child_list = [
		'reverse_two.cpp.htm',
		'hes_times_dir.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
