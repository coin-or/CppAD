// Child table for section asin
document.write('\
<select onchange="asin_child(this)">\
<option>asin-&gt;</option>\
<option>asin.cpp</option>\
</select>\
');
function asin_child(item)
{	var child_list = [
		'asin.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
