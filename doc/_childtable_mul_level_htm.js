// Child table for section mul_level
document.write('\
<select onchange="mul_level_child(this)">\
<option>mul_level-&gt;</option>\
<option>mul_level.cpp</option>\
<option>change_param.cpp</option>\
</select>\
');
function mul_level_child(item)
{	var child_list = [
		'mul_level.cpp.htm',
		'change_param.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
