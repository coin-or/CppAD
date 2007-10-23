#! /bin/bash
#
echo "Assuming ../configure was executed with no arguments,"
echo "This generates a segmentation falut under Cygwin:"
echo
#
cat << EOF > bug0.hpp
# include <cassert>
# include <iostream>
# include <sstream>
# include <string>


# define CPPAD_NULL	       0
# define CPPAD_MAX_NUM_THREADS 1

// TrackElement ------------------------------------------------------------
class TrackElement {
	
public:
	std::string   file;   // corresponding file name
	int           line;   // corresponding line number
	void          *ptr;   // value returned by TrackNew
	TrackElement *next;   // next element in linked list

	// default contructor (used to initialize root)
	TrackElement(void)
	: file(""), line(0), ptr(CPPAD_NULL), next(CPPAD_NULL)
	{ }
	
	TrackElement(const char *f, int l, void *p)
	: file(f), line(l), ptr(p), next(CPPAD_NULL)
	{	assert( p != CPPAD_NULL);
	}

	// There is only one tracking list and it starts it here
	static TrackElement *root_for(size_t thread)
	{	static TrackElement root[CPPAD_MAX_NUM_THREADS];
		assert( thread < CPPAD_MAX_NUM_THREADS );
		return root + thread;
	}

	// There is only one tracking list and it starts it here
	static TrackElement *Root(void)
	{
		size_t thread = 0;
		assert( thread < CPPAD_MAX_NUM_THREADS );
		return root_for(thread); 
	}

}; 

inline void TrackError(void)
{
	std::string str = "test";
	size_t i;
	char message[5];
	for(i = 0; i < 1; i++)
		message[i] = str[i];
	message[1] = '\0';
	assert( false );
}

// TrackNewVec ---------------------------------------------------------------
template <class Type>
Type *TrackNewVec(
	const char *file          , 
	int         line          , 
	size_t      len           ,
	Type       * /* oldptr */ )
{
	// try to allocate the new memrory
	Type *newptr = CPPAD_NULL;
	newptr = new Type[len];
	if( newptr == CPPAD_NULL )
	{	TrackError();
	}
	// create tracking element
	void *vptr = static_cast<void *>(newptr);
	TrackElement *E = new TrackElement(file, line, vptr);

	// get the root
	TrackElement *root = TrackElement::Root();

	// put this elemenent at the front of linked list
	E->next    = root->next;  
	root->next = E;

	return newptr;
}


// TrackDelVec --------------------------------------------------------------

template <class Type>
void TrackDelVec(
	const char *file    ,
	int         line    ,
	Type       *oldptr  )
{
	TrackElement        *P;
	TrackElement        *E;

	// search list for pointer
	P          = TrackElement::Root();
	E          = P->next;
	void *vptr = static_cast<void *>(oldptr);
	while(E != CPPAD_NULL && E->ptr != vptr)
	{	P = E;
		E = E->next;
	}

	// check if pointer was not in list
	if( E == CPPAD_NULL || E->ptr != vptr ) assert(0);

	// remove tracking element from list
	P->next = E->next;

	// delete allocated pointer
	delete [] oldptr;

	// delete tracking element
	delete E;

	return;
}



// TrackCount --------------------------------------------------------------
inline size_t TrackCount(const char *file, int line)
{
	size_t count = 0;
	size_t thread;
	for(thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
	{
		TrackElement *E = TrackElement::root_for(thread);

		while( E->next != CPPAD_NULL ) 
		{	++count;
			E = E->next;
		}
	}
	return count;
}
EOF
cat << EOF >  bug1.cpp
# include "bug0.hpp"
extern bool bug2(void);
int main(void)
{	size_t count = TrackCount(__FILE__, __LINE__);
	std::cout << "count = " <<  count << std::endl;
	bug2();
	return 0;
}
EOF
cat << EOF > bug2.cpp
# include "bug0.hpp"
bool bug2(void)
{	double *x = 0;
	x = TrackNewVec(__FILE__, __LINE__, 1, x);
	x[0] = .4; 
	TrackDelVec(__FILE__, __LINE__, x);
	return true;
}
EOF
echo "g++ bug1.cpp bug2.cpp -O2 -o bug.exe"
g++ bug1.cpp bug2.cpp  -O2 -o bug.exe
echo "./bug"
./bug
