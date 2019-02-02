// Child table for section luratio
document.write('\
<select onchange="luratio_child(this)">\
<option>luratio-&gt;</option>\
<option>lu_ratio.cpp</option>\
</select>\
');
function luratio_child(item)
{	var child_list = [
		'lu_ratio.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
