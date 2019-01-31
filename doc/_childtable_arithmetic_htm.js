// Child table for section arithmetic
document.write('\
<select onchange="arithmetic_child(this)">\
<option>arithmetic-&gt;</option>\
<option>UnaryPlus</option>\
<option>UnaryMinus</option>\
<option>ad_binary</option>\
<option>compound_assign</option>\
</select>\
');
function arithmetic_child(item)
{	var child_list = [
		'unaryplus.htm',
		'unaryminus.htm',
		'ad_binary.htm',
		'compound_assign.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
