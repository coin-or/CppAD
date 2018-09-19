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
'atomic_mat_mul.cpp.htm',
'atomic_mat_mul.hpp.htm'
];
var list_down3 = [
'checkpoint.htm',
'atomic_base.htm',
'atomic_example.htm'
];
var list_down2 = [
'atomic_get_started.cpp.htm',
'atomic_norm_sq.cpp.htm',
'atomic_reciprocal.cpp.htm',
'atomic_set_sparsity.cpp.htm',
'atomic_tangent.cpp.htm',
'atomic_eigen_mat_mul.cpp.htm',
'atomic_eigen_mat_inv.cpp.htm',
'atomic_eigen_cholesky.cpp.htm',
'atomic_mat_mul.cpp.htm',
'atomic_base2ad.cpp.htm'
];
var list_down1 = [
'atomic_mat_mul.hpp.htm'
];
var list_current0 = [
'atomic_mat_mul.hpp.htm#See Also',
'atomic_mat_mul.hpp.htm#Matrix Dimensions',
'atomic_mat_mul.hpp.htm#Start Class Definition',
'atomic_mat_mul.hpp.htm#Constructor',
'atomic_mat_mul.hpp.htm#Left Operand Element Index',
'atomic_mat_mul.hpp.htm#Right Operand Element Index',
'atomic_mat_mul.hpp.htm#Result Element Index',
'atomic_mat_mul.hpp.htm#Forward Matrix Multiply',
'atomic_mat_mul.hpp.htm#Reverse Matrix Multiply',
'atomic_mat_mul.hpp.htm#forward',
'atomic_mat_mul.hpp.htm#reverse',
'atomic_mat_mul.hpp.htm#for_sparse_jac',
'atomic_mat_mul.hpp.htm#rev_sparse_jac',
'atomic_mat_mul.hpp.htm#rev_sparse_hes',
'atomic_mat_mul.hpp.htm#End Class Definition'
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
