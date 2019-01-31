// Child table for section cos
document.write('\
<select onchange="cos_child(this)">\
<option>cos-&gt;</option>\
<option>cos.cpp</option>\
</select>\
');
function cos_child(item)
{	var child_list = [
		'cos.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
