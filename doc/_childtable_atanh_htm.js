// Child table for section atanh
document.write('\
<select onchange="atanh_child(this)">\
<option>atanh-&gt;</option>\
<option>atanh.cpp</option>\
</select>\
');
function atanh_child(item)
{	var child_list = [
		'atanh.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
