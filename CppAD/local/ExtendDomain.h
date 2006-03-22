# ifndef CppADExtendDomainIncluded
# define CppADExtendDomainIncluded

/*
$begin ExtendDomain$$
$spell
	CppAD
	VecAD
	LuRatio
$$


$section Extending the Useful Domain of ADFun Objects$$

$index extend, domain ADFun$$
$index ADFun, domain extend$$
$index domain, extend ADFun$$


$head Description$$
The floating point operations that 
involve $xref/AD//AD<Base>/$$ objects 
can be recorded and transferred to a corresponding 
$xref/ADFun/$$ function object.
On the other hand,
$xref/Compare//comparison/$$ operations are not recorded
(see $xref/ADFun/CompareChange/CompareChange/$$).
The purpose of the routines in this section is 
to transfer comparison operations
(and other operations that are not normally recorded)
in an $code ADFun$$ object
(so as to extend the set of independent variable values for which
the $code ADFun$$ object is useful).

$contents%
	CppAD/local/Discrete.h%
	CppAD/local/Vec.h%
	CppAD/local/LuRatio.h
%$$

$end
*/

# include <CppAD/local/Discrete.h>
# include <CppAD/local/Vec.h>
# include <CppAD/local/LuRatio.h>

# endif
