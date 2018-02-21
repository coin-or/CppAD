var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'utility.htm',
'cppad_vector.htm'
];
var list_down2 = [
'install.htm',
'introduction.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down1 = [
'errorhandler.htm',
'nearequal.htm',
'speed_test.htm',
'speedtest.htm',
'time_test.htm',
'test_boolofvoid.htm',
'numerictype.htm',
'checknumerictype.htm',
'simplevector.htm',
'checksimplevector.htm',
'nan.htm',
'pow_int.htm',
'poly.htm',
'ludetandsolve.htm',
'rombergone.htm',
'rombergmul.htm',
'runge45.htm',
'rosen34.htm',
'odeerrcontrol.htm',
'odegear.htm',
'odegearcontrol.htm',
'cppad_vector.htm',
'thread_alloc.htm',
'index_sort.htm',
'to_string.htm',
'set_union.htm',
'sparse_rc.htm',
'sparse_rcv.htm',
'sparse2eigen.htm'
];
var list_down0 = [
'cppad_vector.cpp.htm',
'vector_bool.cpp.htm'
];
var list_current0 = [
'cppad_vector.htm#Syntax',
'cppad_vector.htm#Description',
'cppad_vector.htm#Include',
'cppad_vector.htm#capacity',
'cppad_vector.htm#Assignment',
'cppad_vector.htm#Assignment.Check Size',
'cppad_vector.htm#Assignment.Return Reference',
'cppad_vector.htm#Assignment.Move Semantics',
'cppad_vector.htm#Element Access',
'cppad_vector.htm#push_back',
'cppad_vector.htm#push_vector',
'cppad_vector.htm#Output',
'cppad_vector.htm#resize',
'cppad_vector.htm#clear',
'cppad_vector.htm#data',
'cppad_vector.htm#vectorBool',
'cppad_vector.htm#vectorBool.Memory',
'cppad_vector.htm#vectorBool.bit_per_unit',
'cppad_vector.htm#vectorBool.data',
'cppad_vector.htm#vectorBool.Output',
'cppad_vector.htm#vectorBool.Element Type',
'cppad_vector.htm#Memory and Parallel Mode',
'cppad_vector.htm#Example',
'cppad_vector.htm#Exercise'
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
