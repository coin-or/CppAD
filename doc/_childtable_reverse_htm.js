// Child table for section reverse
document.write('\
<select onchange="reverse_child(this)">\
<option>reverse-&gt;</option>\
<option>reverse_one</option>\
<option>reverse_two</option>\
<option>reverse_any</option>\
<option>subgraph_reverse</option>\
</select>\
');
function reverse_child(item)
{	var child_list = [
		'reverse_one.htm',
		'reverse_two.htm',
		'reverse_any.htm',
		'subgraph_reverse.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
