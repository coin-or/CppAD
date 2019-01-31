// Child table for section independent
document.write('\
<select onchange="independent_child(this)">\
<option>independent-&gt;</option>\
<option>independent.cpp</option>\
</select>\
');
function independent_child(item)
{	var child_list = [
		'independent.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
