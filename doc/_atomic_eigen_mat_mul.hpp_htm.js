var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'ad.htm',
'advalued.htm',
'atomic.htm',
'atomic_base.htm',
'atomic_eigen_mat_mul.cpp.htm',
'atomic_eigen_mat_mul.hpp.htm'
];
var list_down3 = [
'checkpoint.htm',
'atomic_base.htm'
];
var list_down2 = [
'atomic_ctor.htm',
'atomic_option.htm',
'atomic_afun.htm',
'atomic_forward.htm',
'atomic_reverse.htm',
'atomic_for_sparse_jac.htm',
'atomic_rev_sparse_jac.htm',
'atomic_for_sparse_hes.htm',
'atomic_rev_sparse_hes.htm',
'atomic_base_clear.htm',
'atomic_get_started.cpp.htm',
'atomic_norm_sq.cpp.htm',
'atomic_reciprocal.cpp.htm',
'atomic_set_sparsity.cpp.htm',
'atomic_tangent.cpp.htm',
'atomic_eigen_mat_mul.cpp.htm',
'atomic_eigen_mat_inv.cpp.htm',
'atomic_eigen_cholesky.cpp.htm',
'atomic_mat_mul.cpp.htm'
];
var list_down1 = [
'atomic_eigen_mat_mul.hpp.htm'
];
var list_current0 = [
'atomic_eigen_mat_mul.hpp.htm#See Also',
'atomic_eigen_mat_mul.hpp.htm#Purpose',
'atomic_eigen_mat_mul.hpp.htm#Matrix Dimensions',
'atomic_eigen_mat_mul.hpp.htm#Theory',
'atomic_eigen_mat_mul.hpp.htm#Theory.Forward',
'atomic_eigen_mat_mul.hpp.htm#Theory.Product of Two Matrices',
'atomic_eigen_mat_mul.hpp.htm#Theory.Reverse',
'atomic_eigen_mat_mul.hpp.htm#Start Class Definition',
'atomic_eigen_mat_mul.hpp.htm#Public',
'atomic_eigen_mat_mul.hpp.htm#Public.Types',
'atomic_eigen_mat_mul.hpp.htm#Public.Constructor',
'atomic_eigen_mat_mul.hpp.htm#Public.op',
'atomic_eigen_mat_mul.hpp.htm#Private',
'atomic_eigen_mat_mul.hpp.htm#Private.Variables',
'atomic_eigen_mat_mul.hpp.htm#Private.forward',
'atomic_eigen_mat_mul.hpp.htm#Private.reverse',
'atomic_eigen_mat_mul.hpp.htm#Private.for_sparse_jac',
'atomic_eigen_mat_mul.hpp.htm#Private.rev_sparse_jac',
'atomic_eigen_mat_mul.hpp.htm#Private.for_sparse_hes',
'atomic_eigen_mat_mul.hpp.htm#Private.rev_sparse_hes',
'atomic_eigen_mat_mul.hpp.htm#End Class Definition'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
