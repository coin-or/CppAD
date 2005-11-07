# ifndef CppADExtendBufferIncluded
# define CppADExtendBufferIncluded

/*
$begin ExtendBuffer$$ $comment CppAD Developer Documentation$$

$section Extends the Length of a Buffer$$

$table
$bold Syntax$$ $cnext 
$syntax%ExtendBuffer(size_t &%length%, size_t %number%, %Type% * &%buffer%)%$$
$tend

$fend 20$$

$head Description$$
This function increases the length of the array pointed to by
$italic buffer$$.

$head length$$
is the length of the array pointed to by the new buffer.

$head number$$
is the number of elements in the buffer
that have important values which must be copied to the new buffer.

$head buffer$$
On input, $italic buffer$$ points to  $italic number$$ elements of information
that must be copied from the old buffer to the new one.
On output, $italic buffer$$ points to the new buffer.
The data is copied to the new buffer using the $italic Type$$
assignment operation.

$end
*/

template <typename Type>
void ExtendBuffer(size_t length, size_t Number, Type * &buffer)
{	CppADUnknownError(length > Number);
	Type *newBuffer = CppADNull;
	try
	{	newBuffer = new Type[length];
	}
	catch(...)
	{	CppADUsageError(0, "cannot allocate sufficient memory");
		abort();
	}
	size_t i = Number;
	while(i)
	{	--i;
		newBuffer[i] = buffer[i];
	}
	delete [] buffer;

	buffer  = newBuffer;
} 

# endif
