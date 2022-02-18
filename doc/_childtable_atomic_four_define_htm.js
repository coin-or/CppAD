// Child table for section atomic_four_define
document.write('\
<select onchange="atomic_four_define_child(this)">\
<option>atomic_four_define-&gt;</option>\
<option>atomic_four_ctor</option>\
<option>atomic_four_call</option>\
<option>atomic_four_for_type</option>\
<option>atomic_four_forward</option>\
<option>atomic_four_reverse</option>\
<option>atomic_four_jac_sparsity</option>\
<option>atomic_four_hes_sparsity</option>\
<option>atomic_four_rev_depend</option>\
</select>\
');
function atomic_four_define_child(item)
{	var child_list = [
		'atomic_four_ctor.htm',
		'atomic_four_call.htm',
		'atomic_four_for_type.htm',
		'atomic_four_forward.htm',
		'atomic_four_reverse.htm',
		'atomic_four_jac_sparsity.htm',
		'atomic_four_hes_sparsity.htm',
		'atomic_four_rev_depend.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
