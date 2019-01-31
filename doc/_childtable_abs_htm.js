// Child table for section abs
document.write('\
<select onchange="abs_child(this)">\
<option>abs-&gt;</option>\
<option>fabs.cpp</option>\
</select>\
');
function abs_child(item)
{	var child_list = [
		'fabs.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
