// Child table for section harmonic.cpp
document.write('\
<select onchange="harmonic__46__cpp_child(this)">\
<option>harmonic.cpp-&gt;</option>\
<option>harmonic_common</option>\
<option>harmonic_setup</option>\
<option>harmonic_worker</option>\
<option>harmonic_takedown</option>\
<option>harmonic_sum</option>\
<option>harmonic_time</option>\
</select>\
');
function harmonic__46__cpp_child(item)
{	var child_list = [
		'harmonic_common.htm',
		'harmonic_setup.htm',
		'harmonic_worker.htm',
		'harmonic_takedown.htm',
		'harmonic_sum.htm',
		'harmonic_time.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
