// Child table for section runge45
document.write('\
<select onchange="runge45_child(this)">\
<option>runge45-&gt;</option>\
<option>runge45_1.cpp</option>\
<option>runge_45.cpp</option>\
</select>\
');
function runge45_child(item)
{	var child_list = [
		'runge45_1.cpp.htm',
		'runge_45.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
