// Child table for section pow
document.write('\
<select onchange="pow_child(this)">\
<option>pow-&gt;</option>\
<option>pow.cpp</option>\
</select>\
');
function pow_child(item)
{	var child_list = [
		'pow.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
