// Child table for section utility
document.write('\
<select onchange="utility_child(this)">\
<option>utility-&gt;</option>\
<option>ErrorHandler</option>\
<option>NearEqual</option>\
<option>speed_test</option>\
<option>SpeedTest</option>\
<option>time_test</option>\
<option>test_boolofvoid</option>\
<option>NumericType</option>\
<option>CheckNumericType</option>\
<option>SimpleVector</option>\
<option>CheckSimpleVector</option>\
<option>nan</option>\
<option>pow_int</option>\
<option>Poly</option>\
<option>LuDetAndSolve</option>\
<option>RombergOne</option>\
<option>RombergMul</option>\
<option>Runge45</option>\
<option>Rosen34</option>\
<option>OdeErrControl</option>\
<option>OdeGear</option>\
<option>OdeGearControl</option>\
<option>CppAD_vector</option>\
<option>thread_alloc</option>\
<option>index_sort</option>\
<option>to_string</option>\
<option>set_union</option>\
<option>sparse_rc</option>\
<option>sparse_rcv</option>\
<option>sparse2eigen</option>\
</select>\
');
function utility_child(item)
{	var child_list = [
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
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
