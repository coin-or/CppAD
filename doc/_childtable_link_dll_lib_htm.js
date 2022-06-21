// Child table for section link_dll_lib
document.write('\
<select onchange="link_dll_lib_child(this)">\
<option>link_dll_lib-&gt;</option>\
<option>dll_lib.cpp</option>\
</select>\
');
function link_dll_lib_child(item)
{	var child_list = [
		'dll_lib.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
