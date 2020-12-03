// Child table for section unary_standard_math
document.write('\
<select onchange="unary_standard_math_child(this)">\
<option>unary_standard_math-&gt;</option>\
<option>acos</option>\
<option>acosh</option>\
<option>asin</option>\
<option>asinh</option>\
<option>atan</option>\
<option>atanh</option>\
<option>cos</option>\
<option>cosh</option>\
<option>erf</option>\
<option>erfc</option>\
<option>exp</option>\
<option>expm1</option>\
<option>log</option>\
<option>log1p</option>\
<option>log10</option>\
<option>sin</option>\
<option>sinh</option>\
<option>sqrt</option>\
<option>tan</option>\
<option>tanh</option>\
<option>abs</option>\
<option>sign</option>\
</select>\
');
function unary_standard_math_child(item)
{	var child_list = [
		'acos.htm',
		'acosh.htm',
		'asin.htm',
		'asinh.htm',
		'atan.htm',
		'atanh.htm',
		'cos.htm',
		'cosh.htm',
		'erf.htm',
		'erfc.htm',
		'exp.htm',
		'expm1.htm',
		'log.htm',
		'log1p.htm',
		'log10.htm',
		'sin.htm',
		'sinh.htm',
		'sqrt.htm',
		'tan.htm',
		'tanh.htm',
		'abs.htm',
		'sign.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
