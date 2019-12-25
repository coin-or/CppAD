// Child table for section reverse_any
document.write('\
<select onchange="reverse_any_child(this)">\
<option>reverse_any-&gt;</option>\
<option>reverse_three.cpp</option>\
<option>rev_checkpoint.cpp</option>\
</select>\
');
function reverse_any_child(item)
{	var child_list = [
		'reverse_three.cpp.htm',
		'rev_checkpoint.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
