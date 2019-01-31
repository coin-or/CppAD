// Child table for section lp_box
document.write('\
<select onchange="lp_box_child(this)">\
<option>lp_box-&gt;</option>\
<option>lp_box.cpp</option>\
<option>lp_box.hpp</option>\
</select>\
');
function lp_box_child(item)
{	var child_list = [
		'lp_box.cpp.htm',
		'lp_box.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
