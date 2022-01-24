// Child table for section atomic
document.write('\
<select onchange="atomic_child(this)">\
<option>atomic-&gt;</option>\
<option>atomic_four</option>\
<option>atomic_four_example</option>\
<option>atomic_three</option>\
<option>atomic_three_example</option>\
<option>chkpoint_two</option>\
</select>\
');
function atomic_child(item)
{	var child_list = [
		'atomic_four.htm',
		'atomic_four_example.htm',
		'atomic_three.htm',
		'atomic_three_example.htm',
		'chkpoint_two.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
