// Child table for section example
document.write('\
<select onchange="example_child(this)">\
<option>example-&gt;</option>\
<option>get_started.cpp</option>\
<option>General</option>\
<option>ExampleUtility</option>\
<option>ListAllExamples</option>\
<option>testvector</option>\
<option>wno_conversion</option>\
</select>\
');
function example_child(item)
{	var child_list = [
		'get_started.cpp.htm',
		'general.htm',
		'exampleutility.htm',
		'listallexamples.htm',
		'testvector.htm',
		'wno_conversion.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
