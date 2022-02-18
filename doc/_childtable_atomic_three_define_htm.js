// Child table for section atomic_three_define
document.write('\
<select onchange="atomic_three_define_child(this)">\
<option>atomic_three_define-&gt;</option>\
<option>atomic_three_ctor</option>\
<option>atomic_three_afun</option>\
<option>atomic_three_for_type</option>\
<option>atomic_three_forward</option>\
<option>atomic_three_reverse</option>\
<option>atomic_three_jac_sparsity</option>\
<option>atomic_three_hes_sparsity</option>\
<option>atomic_three_rev_depend</option>\
</select>\
');
function atomic_three_define_child(item)
{	var child_list = [
		'atomic_three_ctor.htm',
		'atomic_three_afun.htm',
		'atomic_three_for_type.htm',
		'atomic_three_forward.htm',
		'atomic_three_reverse.htm',
		'atomic_three_jac_sparsity.htm',
		'atomic_three_hes_sparsity.htm',
		'atomic_three_rev_depend.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
