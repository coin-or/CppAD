// Child table for section equalopseq
document.write('\
<select onchange="equalopseq_child(this)">\
<option>equalopseq-&gt;</option>\
<option>equal_op_seq.cpp</option>\
</select>\
');
function equalopseq_child(item)
{	var child_list = [
		'equal_op_seq.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
