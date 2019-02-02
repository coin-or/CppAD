// Child table for section ad_ctor
document.write('\
<select onchange="ad_ctor_child(this)">\
<option>ad_ctor-&gt;</option>\
<option>ad_ctor.cpp</option>\
</select>\
');
function ad_ctor_child(item)
{	var child_list = [
		'ad_ctor.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
