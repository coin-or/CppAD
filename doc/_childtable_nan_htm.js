// Child table for section nan
document.write('\
<select onchange="nan_child(this)">\
<option>nan-&gt;</option>\
<option>nan.cpp</option>\
</select>\
');
function nan_child(item)
{	var child_list = [
		'nan.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
