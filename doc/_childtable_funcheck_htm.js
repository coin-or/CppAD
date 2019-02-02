// Child table for section funcheck
document.write('\
<select onchange="funcheck_child(this)">\
<option>funcheck-&gt;</option>\
<option>fun_check.cpp</option>\
</select>\
');
function funcheck_child(item)
{	var child_list = [
		'fun_check.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
