// Child table for section checksimplevector
document.write('\
<select onchange="checksimplevector_child(this)">\
<option>checksimplevector-&gt;</option>\
<option>check_simple_vector.cpp</option>\
</select>\
');
function checksimplevector_child(item)
{	var child_list = [
		'check_simple_vector.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
