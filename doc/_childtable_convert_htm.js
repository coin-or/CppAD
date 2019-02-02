// Child table for section convert
document.write('\
<select onchange="convert_child(this)">\
<option>convert-&gt;</option>\
<option>Value</option>\
<option>Integer</option>\
<option>ad_to_string</option>\
<option>ad_input</option>\
<option>ad_output</option>\
<option>PrintFor</option>\
<option>Var2Par</option>\
</select>\
');
function convert_child(item)
{	var child_list = [
		'value.htm',
		'integer.htm',
		'ad_to_string.htm',
		'ad_input.htm',
		'ad_output.htm',
		'printfor.htm',
		'var2par.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
