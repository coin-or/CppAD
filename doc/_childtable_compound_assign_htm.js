// Child table for section compound_assign
document.write('\
<select onchange="compound_assign_child(this)">\
<option>compound_assign-&gt;</option>\
<option>AddEq.cpp</option>\
<option>sub_eq.cpp</option>\
<option>mul_eq.cpp</option>\
<option>div_eq.cpp</option>\
</select>\
');
function compound_assign_child(item)
{	var child_list = [
		'addeq.cpp.htm',
		'sub_eq.cpp.htm',
		'mul_eq.cpp.htm',
		'div_eq.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
