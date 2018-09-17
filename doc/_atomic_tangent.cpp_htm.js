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
'atomic_example.htm',
'atomic_tangent.cpp.htm'
];
var list_down3 = [
'arithmetic.htm',
'unary_standard_math.htm',
'binary_math.htm',
'condexp.htm',
'discrete.htm',
'numeric_limits.htm',
'atomic.htm'
];
var list_down2 = [
'checkpoint.htm',
'atomic_base.htm',
'atomic_example.htm'
];
var list_down1 = [
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
var list_current0 = [
'atomic_tangent.cpp.htm#Theory',
'atomic_tangent.cpp.htm#sparsity',
'atomic_tangent.cpp.htm#Start Class Definition',
'atomic_tangent.cpp.htm#Constructor',
'atomic_tangent.cpp.htm#forward',
'atomic_tangent.cpp.htm#reverse',
'atomic_tangent.cpp.htm#for_sparse_jac',
'atomic_tangent.cpp.htm#rev_sparse_jac',
'atomic_tangent.cpp.htm#rev_sparse_hes',
'atomic_tangent.cpp.htm#End Class Definition',
'atomic_tangent.cpp.htm#Use Atomic Function',
'atomic_tangent.cpp.htm#Use Atomic Function.Constructor',
'atomic_tangent.cpp.htm#Use Atomic Function.Recording',
'atomic_tangent.cpp.htm#Use Atomic Function.forward',
'atomic_tangent.cpp.htm#Use Atomic Function.reverse',
'atomic_tangent.cpp.htm#Use Atomic Function.for_sparse_jac',
'atomic_tangent.cpp.htm#Use Atomic Function.rev_sparse_jac',
'atomic_tangent.cpp.htm#Use Atomic Function.rev_sparse_hes',
'atomic_tangent.cpp.htm#Use Atomic Function.Large x Values'
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
