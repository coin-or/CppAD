// Child table for section asinh
document.write('\
<select onchange="asinh_child(this)">\
<option>asinh-&gt;</option>\
<option>asinh.cpp</option>\
</select>\
');
function asinh_child(item)
{	var child_list = [
		'asinh.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
