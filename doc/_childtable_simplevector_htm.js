// Child table for section simplevector
document.write('\
<select onchange="simplevector_child(this)">\
<option>simplevector-&gt;</option>\
<option>simple_vector.cpp</option>\
</select>\
');
function simplevector_child(item)
{	var child_list = [
		'simple_vector.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
