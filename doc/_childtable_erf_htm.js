// Child table for section erf
document.write('\
<select onchange="erf_child(this)">\
<option>erf-&gt;</option>\
<option>erf.cpp</option>\
</select>\
');
function erf_child(item)
{	var child_list = [
		'erf.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
