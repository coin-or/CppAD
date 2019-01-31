// Child table for section abort_recording
document.write('\
<select onchange="abort_recording_child(this)">\
<option>abort_recording-&gt;</option>\
<option>abort_recording.cpp</option>\
</select>\
');
function abort_recording_child(item)
{	var child_list = [
		'abort_recording.cpp.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
