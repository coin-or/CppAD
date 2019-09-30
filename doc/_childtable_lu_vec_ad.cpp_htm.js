// Child table for section lu_vec_ad.cpp
document.write('\
<select onchange="lu_vec_ad__46__cpp_child(this)">\
<option>lu_vec_ad.cpp-&gt;</option>\
<option>lu_vec_ad_ok.cpp</option>\
</select>\
');
function lu_vec_ad__46__cpp_child(item)
{	var child_list = [
		'lu_vec_ad_ok.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
