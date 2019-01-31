// Child table for section set_union
document.write('\
<select onchange="set_union_child(this)">\
<option>set_union-&gt;</option>\
<option>set_union.cpp</option>\
</select>\
');
function set_union_child(item)
{	var child_list = [
		'set_union.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
