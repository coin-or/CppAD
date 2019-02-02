// Child table for section base_require
document.write('\
<select onchange="base_require_child(this)">\
<option>base_require-&gt;</option>\
<option>base_member</option>\
<option>base_cond_exp</option>\
<option>base_identical</option>\
<option>base_ordered</option>\
<option>base_std_math</option>\
<option>base_limits</option>\
<option>base_to_string</option>\
<option>base_hash</option>\
<option>base_example</option>\
</select>\
');
function base_require_child(item)
{	var child_list = [
		'base_member.htm',
		'base_cond_exp.htm',
		'base_identical.htm',
		'base_ordered.htm',
		'base_std_math.htm',
		'base_limits.htm',
		'base_to_string.htm',
		'base_hash.htm',
		'base_example.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
