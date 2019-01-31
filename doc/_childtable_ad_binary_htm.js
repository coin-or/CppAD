// Child table for section ad_binary
document.write('\
<select onchange="ad_binary_child(this)">\
<option>ad_binary-&gt;</option>\
<option>add.cpp</option>\
<option>sub.cpp</option>\
<option>mul.cpp</option>\
<option>div.cpp</option>\
</select>\
');
function ad_binary_child(item)
{	var child_list = [
		'add.cpp.htm',
		'sub.cpp.htm',
		'mul.cpp.htm',
		'div.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
