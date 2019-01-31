// Child table for section ad_input
document.write('\
<select onchange="ad_input_child(this)">\
<option>ad_input-&gt;</option>\
<option>ad_input.cpp</option>\
</select>\
');
function ad_input_child(item)
{	var child_list = [
		'ad_input.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
