// Child table for section atan
document.write('\
<select onchange="atan_child(this)">\
<option>atan-&gt;</option>\
<option>atan.cpp</option>\
</select>\
');
function atan_child(item)
{	var child_list = [
		'atan.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
