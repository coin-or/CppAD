// Child table for section cppad_vector
document.write('\
<select onchange="cppad_vector_child(this)">\
<option>cppad_vector-&gt;</option>\
<option>cppad_vector.cpp</option>\
<option>vector_bool.cpp</option>\
</select>\
');
function cppad_vector_child(item)
{	var child_list = [
		'cppad_vector.cpp.htm',
		'vector_bool.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
