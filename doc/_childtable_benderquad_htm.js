// Child table for section benderquad
document.write('\
<select onchange="benderquad_child(this)">\
<option>benderquad-&gt;</option>\
<option>bender_quad.cpp</option>\
</select>\
');
function benderquad_child(item)
{	var child_list = [
		'bender_quad.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
