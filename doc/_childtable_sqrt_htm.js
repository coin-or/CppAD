// Child table for section sqrt
document.write('\
<select onchange="sqrt_child(this)">\
<option>sqrt-&gt;</option>\
<option>sqrt.cpp</option>\
</select>\
');
function sqrt_child(item)
{	var child_list = [
		'sqrt.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
