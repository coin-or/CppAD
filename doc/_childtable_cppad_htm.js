// Child table for section cppad
document.write('\
<select onchange="cppad_child(this)">\
<option>cppad-&gt;</option>\
<option>Install</option>\
<option>Theory</option>\
<option>AD</option>\
<option>ADFun</option>\
<option>preprocessor</option>\
<option>multi_thread</option>\
<option>utility</option>\
<option>ipopt_solve</option>\
<option>Example</option>\
<option>speed</option>\
<option>Appendix</option>\
<option>_reference</option>\
<option>_index</option>\
<option>_search</option>\
<option>_external</option>\
</select>\
');
function cppad_child(item)
{	var child_list = [
		'install.htm',
		'theory.htm',
		'ad.htm',
		'adfun.htm',
		'preprocessor.htm',
		'multi_thread.htm',
		'utility.htm',
		'ipopt_solve.htm',
		'example.htm',
		'speed.htm',
		'appendix.htm',
		'_reference.htm',
		'_index.htm',
		'_search.htm',
		'_external.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
