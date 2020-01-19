// Child table for section cmake
document.write('\
<select onchange="cmake_child(this)">\
<option>cmake-&gt;</option>\
<option>get_optional.sh</option>\
<option>adolc_prefix</option>\
<option>colpack_prefix</option>\
<option>eigen_prefix</option>\
<option>fadbad_prefix</option>\
<option>ipopt_prefix</option>\
<option>sacado_prefix</option>\
<option>cppad_testvector</option>\
</select>\
');
function cmake_child(item)
{	var child_list = [
		'get_optional.sh.htm',
		'adolc_prefix.htm',
		'colpack_prefix.htm',
		'eigen_prefix.htm',
		'fadbad_prefix.htm',
		'ipopt_prefix.htm',
		'sacado_prefix.htm',
		'cppad_testvector.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
