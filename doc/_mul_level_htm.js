var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'example.htm',
'general.htm',
'mul_level.htm'
];
var list_down3 = [
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
var list_down2 = [
'get_started.cpp.htm',
'general.htm',
'exampleutility.htm',
'listallexamples.htm',
'testvector.htm',
'wno_conversion.htm'
];
var list_down1 = [
'ad_fun.cpp.htm',
'ad_in_c.cpp.htm',
'conj_grad.cpp.htm',
'cppad_eigen.hpp.htm',
'hes_minor_det.cpp.htm',
'hes_lu_det.cpp.htm',
'interface2c.cpp.htm',
'jac_minor_det.cpp.htm',
'jac_lu_det.cpp.htm',
'mul_level.htm',
'ode_stiff.cpp.htm',
'mul_level_ode.cpp.htm',
'mul_level_adolc_ode.cpp.htm',
'stack_machine.cpp.htm'
];
var list_down0 = [
'mul_level.cpp.htm',
'change_param.cpp.htm'
];
var list_current0 = [
'mul_level.htm#Background',
'mul_level.htm#Motivation',
'mul_level.htm#Procedure',
'mul_level.htm#Procedure.First Start AD&lt;double&gt;',
'mul_level.htm#Procedure.Start AD&lt; AD&lt;double&gt; &gt; Recording',
'mul_level.htm#Procedure.Inner Function',
'mul_level.htm#Procedure.Second Start AD&lt; AD&lt;double&gt; &gt;',
'mul_level.htm#Procedure.Outer Function',
'mul_level.htm#Procedure.Derivatives of Outer Function',
'mul_level.htm#Example'
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
