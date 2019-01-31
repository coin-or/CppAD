// Child table for section revone
document.write('\
<select onchange="revone_child(this)">\
<option>revone-&gt;</option>\
<option>rev_one.cpp</option>\
</select>\
');
function revone_child(item)
{	var child_list = [
		'rev_one.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
