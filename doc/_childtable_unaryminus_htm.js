// Child table for section unaryminus
document.write('\
<select onchange="unaryminus_child(this)">\
<option>unaryminus-&gt;</option>\
<option>unary_minus.cpp</option>\
</select>\
');
function unaryminus_child(item)
{	var child_list = [
		'unary_minus.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
