// Child table for section revtwo
document.write('\
<select onchange="revtwo_child(this)">\
<option>revtwo-&gt;</option>\
<option>rev_two.cpp</option>\
</select>\
');
function revtwo_child(item)
{	var child_list = [
		'rev_two.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
