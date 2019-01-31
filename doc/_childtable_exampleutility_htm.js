// Child table for section exampleutility
document.write('\
<select onchange="exampleutility_child(this)">\
<option>exampleutility-&gt;</option>\
<option>general.cpp</option>\
<option>speed_example.cpp</option>\
<option>lu_vec_ad.cpp</option>\
</select>\
');
function exampleutility_child(item)
{	var child_list = [
		'general.cpp.htm',
		'speed_example.cpp.htm',
		'lu_vec_ad.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
