// Child table for section compare
document.write('\
<select onchange="compare_child(this)">\
<option>compare-&gt;</option>\
<option>compare.cpp</option>\
</select>\
');
function compare_child(item)
{	var child_list = [
		'compare.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
