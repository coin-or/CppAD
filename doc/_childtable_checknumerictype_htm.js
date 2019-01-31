// Child table for section checknumerictype
document.write('\
<select onchange="checknumerictype_child(this)">\
<option>checknumerictype-&gt;</option>\
<option>check_numeric_type.cpp</option>\
</select>\
');
function checknumerictype_child(item)
{	var child_list = [
		'check_numeric_type.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
