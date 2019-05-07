// Child table for section unary_standard_math
document.write('\
<select onchange="unary_standard_math_child(this)">\
<option>unary_standard_math-&gt;</option>\
<option>acos</option>\
<option>asin</option>\
<option>atan</option>\
<option>cos</option>\
<option>cosh</option>\
<option>exp</option>\
<option>log</option>\
<option>log10</option>\
<option>sin</option>\
<option>sinh</option>\
<option>sqrt</option>\
<option>tan</option>\
<option>tanh</option>\
<option>abs</option>\
<option>acosh</option>\
<option>asinh</option>\
<option>atanh</option>\
<option>erf</option>\
<option>erfc</option>\
<option>expm1</option>\
<option>log1p</option>\
<option>sign</option>\
</select>\
');
function unary_standard_math_child(item)
{	var child_list = [
		'acos.htm',
		'asin.htm',
		'atan.htm',
		'cos.htm',
		'cosh.htm',
		'exp.htm',
		'log.htm',
		'log10.htm',
		'sin.htm',
		'sinh.htm',
		'sqrt.htm',
		'tan.htm',
		'tanh.htm',
		'abs.htm',
		'acosh.htm',
		'asinh.htm',
		'atanh.htm',
		'erf.htm',
		'erfc.htm',
		'expm1.htm',
		'log1p.htm',
		'sign.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
