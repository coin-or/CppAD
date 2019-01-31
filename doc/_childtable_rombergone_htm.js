// Child table for section rombergone
document.write('\
<select onchange="rombergone_child(this)">\
<option>rombergone-&gt;</option>\
<option>romberg_one.cpp</option>\
</select>\
');
function rombergone_child(item)
{	var child_list = [
		'romberg_one.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
