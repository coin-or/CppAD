// Child table for section tanh
document.write('\
<select onchange="tanh_child(this)">\
<option>tanh-&gt;</option>\
<option>tanh.cpp</option>\
</select>\
');
function tanh_child(item)
{	var child_list = [
		'tanh.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
