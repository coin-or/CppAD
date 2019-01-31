// Child table for section unaryplus
document.write('\
<select onchange="unaryplus_child(this)">\
<option>unaryplus-&gt;</option>\
<option>unary_plus.cpp</option>\
</select>\
');
function unaryplus_child(item)
{	var child_list = [
		'unary_plus.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
