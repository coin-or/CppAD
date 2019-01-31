// Child table for section compare_c
document.write('\
<select onchange="compare_c_child(this)">\
<option>compare_c-&gt;</option>\
<option>det_of_minor_c</option>\
<option>det_by_minor_c</option>\
<option>uniform_01_c</option>\
<option>correct_det_by_minor_c</option>\
<option>repeat_det_by_minor_c</option>\
<option>elapsed_seconds_c</option>\
<option>time_det_by_minor_c</option>\
<option>main_compare_c</option>\
</select>\
');
function compare_c_child(item)
{	var child_list = [
		'det_of_minor_c.htm',
		'det_by_minor_c.htm',
		'uniform_01_c.htm',
		'correct_det_by_minor_c.htm',
		'repeat_det_by_minor_c.htm',
		'elapsed_seconds_c.htm',
		'time_det_by_minor_c.htm',
		'main_compare_c.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
