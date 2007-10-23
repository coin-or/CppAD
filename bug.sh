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


// TrackElement ------------------------------------------------------------
class TrackElement {
	
public:
	std::string   file;   // corresponding file name
	void          *ptr;   // value returned by TrackNew
	TrackElement *next;   // next element in linked list

	TrackElement(void *p)
	: file(""), ptr(p), next(0)
	{ }

	// There is only one tracking list and it starts it here
	static TrackElement *root_for(void)
	{	static TrackElement root(0);
		return &root;
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
Type *TrackNewVec(size_t len , Type *oldptr)
{
	// try to allocate the new memrory
	Type *newptr = new Type[len];
	if( newptr == 0 )
	{	TrackError();
	}
	// create tracking element
	void *vptr = static_cast<void *>(newptr);
	TrackElement *E = new TrackElement(vptr);

	// get the root
	TrackElement *root = TrackElement::root_for();

	// put this elemenent at the front of linked list
	E->next    = root->next;  
	root->next = E;

	return newptr;
}


// TrackDelVec --------------------------------------------------------------

template <class Type>
void TrackDelVec(Type *oldptr )
{
	TrackElement        *P;
	TrackElement        *E;

	// search list for pointer
	P          = TrackElement::root_for();
	E          = P->next;
	void *vptr = static_cast<void *>(oldptr);
	while(E != 0 && E->ptr != vptr)
	{	P = E;
		E = E->next;
	}

	// check if pointer was not in list
	if( E == 0 || E->ptr != vptr ) assert(0);

	// remove tracking element from list
	P->next = E->next;

	// delete allocated pointer
	delete [] oldptr;

	// delete tracking element
	delete E;

	return;
}



// TrackCount --------------------------------------------------------------
inline size_t TrackCount(void)
{
	size_t count = 0;
	TrackElement *E = TrackElement::root_for();

	while( E->next != 0 ) 
	{	++count;
		E = E->next;
	}
	return count;
}
EOF
cat << EOF >  bug1.cpp
# include "bug0.hpp"
extern bool bug2(void);
int main(void)
{	size_t count = TrackCount();
	std::cout << "count = " <<  count << std::endl;
	bug2();
	return 0;
}
EOF
cat << EOF > bug2.cpp
# include "bug0.hpp"
bool bug2(void)
{	double *x = 0;
	x = TrackNewVec(1, x);
	x[0] = .4; 
	TrackDelVec(x);
	return true;
}
EOF
echo "g++ bug1.cpp bug2.cpp -O2 -o bug.exe"
g++ bug1.cpp bug2.cpp  -O2 -o bug.exe
echo "./bug"
./bug
