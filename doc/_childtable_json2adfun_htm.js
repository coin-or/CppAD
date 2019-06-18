// Child table for section json2adfun
document.write('\
<select onchange="json2adfun_child(this)">\
<option>json2adfun-&gt;</option>\
<option>json2adfun.cpp</option>\
</select>\
');
function json2adfun_child(item)
{	var child_list = [
		'json2adfun.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
