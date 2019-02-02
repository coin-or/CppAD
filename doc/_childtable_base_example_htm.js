// Child table for section base_example
document.write('\
<select onchange="base_example_child(this)">\
<option>base_example-&gt;</option>\
<option>base_alloc.hpp</option>\
<option>base_require.cpp</option>\
<option>base_adolc.hpp</option>\
<option>base_float.hpp</option>\
<option>base_double.hpp</option>\
<option>base_complex.hpp</option>\
</select>\
');
function base_example_child(item)
{	var child_list = [
		'base_alloc.hpp.htm',
		'base_require.cpp.htm',
		'base_adolc.hpp.htm',
		'base_float.hpp.htm',
		'base_double.hpp.htm',
		'base_complex.hpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
