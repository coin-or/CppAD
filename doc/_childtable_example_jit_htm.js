// Child table for section example_jit
document.write('\
<select onchange="example_jit_child(this)">\
<option>example_jit-&gt;</option>\
<option>jit_get_started.cpp</option>\
<option>jit_to_csrc.cpp</option>\
</select>\
');
function example_jit_child(item)
{	var child_list = [
		'jit_get_started.cpp.htm',
		'jit_to_csrc.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
