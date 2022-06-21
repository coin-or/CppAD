// Child table for section utility
document.write('\
<select onchange="utility_child(this)">\
<option>utility-&gt;</option>\
<option>CheckNumericType</option>\
<option>CheckSimpleVector</option>\
<option>create_dll_lib</option>\
<option>ErrorHandler</option>\
<option>index_sort</option>\
<option>link_dll_lib</option>\
<option>nan</option>\
<option>NearEqual</option>\
<option>OdeErrControl</option>\
<option>OdeGear</option>\
<option>OdeGearControl</option>\
<option>CppAD_vector</option>\
<option>Poly</option>\
<option>pow_int</option>\
<option>RombergMul</option>\
<option>RombergOne</option>\
<option>Rosen34</option>\
<option>Runge45</option>\
<option>set_union</option>\
<option>sparse2eigen</option>\
<option>sparse_rc</option>\
<option>sparse_rcv</option>\
<option>speed_test</option>\
<option>SpeedTest</option>\
<option>test_boolofvoid</option>\
<option>time_test</option>\
<option>to_string</option>\
<option>LuDetAndSolve</option>\
<option>NumericType</option>\
<option>SimpleVector</option>\
<option>thread_alloc</option>\
</select>\
');
function utility_child(item)
{	var child_list = [
		'checknumerictype.htm',
		'checksimplevector.htm',
		'create_dll_lib.htm',
		'errorhandler.htm',
		'index_sort.htm',
		'link_dll_lib.htm',
		'nan.htm',
		'nearequal.htm',
		'odeerrcontrol.htm',
		'odegear.htm',
		'odegearcontrol.htm',
		'cppad_vector.htm',
		'poly.htm',
		'pow_int.htm',
		'rombergmul.htm',
		'rombergone.htm',
		'rosen34.htm',
		'runge45.htm',
		'set_union.htm',
		'sparse2eigen.htm',
		'sparse_rc.htm',
		'sparse_rcv.htm',
		'speed_test.htm',
		'speedtest.htm',
		'test_boolofvoid.htm',
		'time_test.htm',
		'to_string.htm',
		'ludetandsolve.htm',
		'numerictype.htm',
		'simplevector.htm',
		'thread_alloc.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
