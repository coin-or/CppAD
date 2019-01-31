// Child table for section rosen34
document.write('\
<select onchange="rosen34_child(this)">\
<option>rosen34-&gt;</option>\
<option>rosen_34.cpp</option>\
</select>\
');
function rosen34_child(item)
{	var child_list = [
		'rosen_34.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
