// Child table for section cosh
document.write('\
<select onchange="cosh_child(this)">\
<option>cosh-&gt;</option>\
<option>cosh.cpp</option>\
</select>\
');
function cosh_child(item)
{	var child_list = [
		'cosh.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
