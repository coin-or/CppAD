// Child table for section expm1
document.write('\
<select onchange="expm1_child(this)">\
<option>expm1-&gt;</option>\
<option>expm1.cpp</option>\
</select>\
');
function expm1_child(item)
{	var child_list = [
		'expm1.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
