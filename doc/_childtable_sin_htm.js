// Child table for section sin
document.write('\
<select onchange="sin_child(this)">\
<option>sin-&gt;</option>\
<option>sin.cpp</option>\
</select>\
');
function sin_child(item)
{	var child_list = [
		'sin.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
