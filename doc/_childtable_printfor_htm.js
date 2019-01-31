// Child table for section printfor
document.write('\
<select onchange="printfor_child(this)">\
<option>printfor-&gt;</option>\
<option>print_for_cout.cpp</option>\
<option>print_for_string.cpp</option>\
</select>\
');
function printfor_child(item)
{	var child_list = [
		'print_for_cout.cpp.htm',
		'print_for_string.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
