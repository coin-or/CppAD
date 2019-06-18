// Child table for section record_adfun
document.write('\
<select onchange="record_adfun_child(this)">\
<option>record_adfun-&gt;</option>\
<option>Independent</option>\
<option>FunConstruct</option>\
<option>Dependent</option>\
<option>abort_recording</option>\
<option>seq_property</option>\
</select>\
');
function record_adfun_child(item)
{	var child_list = [
		'independent.htm',
		'funconstruct.htm',
		'dependent.htm',
		'abort_recording.htm',
		'seq_property.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
