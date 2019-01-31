// Child table for section vecad
document.write('\
<select onchange="vecad_child(this)">\
<option>vecad-&gt;</option>\
<option>vec_ad.cpp</option>\
</select>\
');
function vecad_child(item)
{	var child_list = [
		'vec_ad.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
