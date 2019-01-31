// Child table for section ad_output
document.write('\
<select onchange="ad_output_child(this)">\
<option>ad_output-&gt;</option>\
<option>ad_output.cpp</option>\
</select>\
');
function ad_output_child(item)
{	var child_list = [
		'ad_output.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
