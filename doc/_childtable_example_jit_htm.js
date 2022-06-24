// Child table for section example_jit
document.write('\
<select onchange="example_jit_child(this)">\
<option>example_jit-&gt;</option>\
<option>jit_get_started.cpp</option>\
<option>jit_to_csrc.cpp</option>\
<option>jit_compile.cpp</option>\
<option>jit_atomic.cpp</option>\
<option>jit_dynamic.cpp</option>\
</select>\
');
function example_jit_child(item)
{	var child_list = [
		'jit_get_started.cpp.htm',
		'jit_to_csrc.cpp.htm',
		'jit_compile.cpp.htm',
		'jit_atomic.cpp.htm',
		'jit_dynamic.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
