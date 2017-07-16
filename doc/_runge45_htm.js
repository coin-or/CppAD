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
'runge45.htm'
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
'sparse_rcv.htm'
];
var list_down0 = [
'runge45_1.cpp.htm',
'runge45_2.cpp.htm'
];
var list_current0 = [
'runge45.htm#Syntax',
'runge45.htm#Purpose',
'runge45.htm#Operation Sequence',
'runge45.htm#Include',
'runge45.htm#xf',
'runge45.htm#Fun',
'runge45.htm#Fun.t',
'runge45.htm#Fun.x',
'runge45.htm#Fun.f',
'runge45.htm#Fun.Warning',
'runge45.htm#M',
'runge45.htm#ti',
'runge45.htm#tf',
'runge45.htm#xi',
'runge45.htm#e',
'runge45.htm#Scalar',
'runge45.htm#Scalar.fabs',
'runge45.htm#Vector',
'runge45.htm#Parallel Mode',
'runge45.htm#Example',
'runge45.htm#Source Code'
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
