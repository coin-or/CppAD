// Child table for section atomic_two
document.write('\
<select onchange="atomic_two_child(this)">\
<option>atomic_two-&gt;</option>\
<option>atomic_two_ctor</option>\
<option>atomic_two_option</option>\
<option>atomic_two_afun</option>\
<option>atomic_two_forward</option>\
<option>atomic_two_reverse</option>\
<option>atomic_two_for_sparse_jac</option>\
<option>atomic_two_rev_sparse_jac</option>\
<option>atomic_two_for_sparse_hes</option>\
<option>atomic_two_rev_sparse_hes</option>\
<option>atomic_two_clear</option>\
</select>\
');
function atomic_two_child(item)
{	var child_list = [
		'atomic_two_ctor.htm',
		'atomic_two_option.htm',
		'atomic_two_afun.htm',
		'atomic_two_forward.htm',
		'atomic_two_reverse.htm',
		'atomic_two_for_sparse_jac.htm',
		'atomic_two_rev_sparse_jac.htm',
		'atomic_two_for_sparse_hes.htm',
		'atomic_two_rev_sparse_hes.htm',
		'atomic_two_clear.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
