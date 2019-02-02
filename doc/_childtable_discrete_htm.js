// Child table for section discrete
document.write('\
<select onchange="discrete_child(this)">\
<option>discrete-&gt;</option>\
<option>tape_index.cpp</option>\
<option>interp_onetape.cpp</option>\
<option>interp_retape.cpp</option>\
</select>\
');
function discrete_child(item)
{	var child_list = [
		'tape_index.cpp.htm',
		'interp_onetape.cpp.htm',
		'interp_retape.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
