// Child table for section integer
document.write('\
<select onchange="integer_child(this)">\
<option>integer-&gt;</option>\
<option>integer.cpp</option>\
</select>\
');
function integer_child(item)
{	var child_list = [
		'integer.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
