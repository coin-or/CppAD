// Child table for section check_for_nan
document.write('\
<select onchange="check_for_nan_child(this)">\
<option>check_for_nan-&gt;</option>\
<option>check_for_nan.cpp</option>\
</select>\
');
function check_for_nan_child(item)
{	var child_list = [
		'check_for_nan.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
