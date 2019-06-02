# ifndef CPPAD_LOCAL_SPARSE_LIST_SETVEC_HPP
# define CPPAD_LOCAL_SPARSE_LIST_SETVEC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/local/define.hpp>
# include <cppad/local/is_pod.hpp>
# include <list>

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {
/*!
\file list_setvec.hpp
Vector of sets of positive integers stored as singly linked lists
with the element values strictly increasing.
*/
class list_setvec_const_iterator;

/*
Vector of sets of positive integers, each set stored as a singly
linked list.

All the public members for this class are also in the
sparse::pack_setvec and sparse_vecsize classes.
This defines the CppAD vector_of_sets concept.
*/
class list_setvec {
    friend class list_setvec_const_iterator;
/*
-------------------------------------------------------------------------------
$begin list_setvec_member_data$$
$spell
    setvec
    struct
$$

$section class list_setvec: Member Data$$

$head pair_size_t$$
This $code struct$$ is local to the $code list_setvec$$ class.
It is the type used for each entry in a singly linked list
and has the following fields:

$subhead value$$
The is the value of an entry in the list
(for sets, the first entry in the list is a reference count).

$subhead next$$
This is the index in $code data_$$ for the next entry in the list.
If there are no more entries in the list, this value is zero; i.e.,
$code data_[0]$$ is used to represent the end of a list.

$head end_$$
The possible elements in each set are $code 0$$, $code 1$$, ...,
$code end_-1$$

$head number_not_used_$$
Number of elements of the $code data_$$ vector that are not being used.

$head data_not_used_$$
Index in $code data_$$ for the start of the linked list of elements
that are not being used.

$head data_$$
The data for all the singly linked lists.
If $icode%n_set% > 0%$$,
$codei%data[0].value == end_%$$ and
$codei%data[0].next == 0%$$.

$head start_$$
The size of this vector is the number of set; i.e.,
$cref/n_set/SetVector/Vector Operations/n_set/$$.
The starting index for $th i$$ set is $codei%start_[%i%]%$$.
If $codei%start_[%i%] == 0%$$, the i-th set has no elements.
Otherwise it is the index of the reference count for the list.

$subhead Reference Count$$
If $codei%start_[%i%] != 0%$$, $codei%data_[start_[%i%]].value%$$
is the reference count for the $th i$$ list
(not the value of an element in the list).
The reference count must be greater than zero.

$subhead First Element$$
If $codei%start_[%i%] != 0%$$,
$codei%
    %first_index% = data_[start_[%i%]].next
%$$
is the index of the first element in the list.
This must be non-zero because the list is empty.

$subhead Next Element$$
Starting with $icode%index% = %first_index%$$,
while $icode%index% != 0%$$,
The value of the corresponding element in the list is
$codei%data_[%index%].value%$$ (which must be less than $code end_$$).
The $icode index$$ for the next element of the list is
$codei%data_[%index%].next%$$.

$subhead Last Element$$
An index $icode last$$ corresponds to the last element of a list if
$codei%data_[%last%].next == 0%$$.
(Note that $code data_[0].value == end_$$).

$head post_$$
The size of this vector is the number of set; i.e.,
$cref/n_set/SetVector/Vector Operations/n_set/$$.
Starting with $icode%index% = post_[%i%]%$$,
while $icode%index% != 0%$$,
The value of the next element posted to the $th i$$ list is
$codei%data_[%index%].value%$$ (which must be less than $code end_$$).
The $icode index$$ for the next element posted to the $th i$$ list is
$codei%data_[%index%].next%$$.

$head temporary_$$
A temporary vector used by member functions that keeps its capacity
(to re-allocating memory).

$head Source Code$$
$srccode%hpp% */
private:
    struct pair_size_t {size_t value; size_t next; };
    friend bool CppAD::local::is_pod<pair_size_t>(void);
    //
    size_t                   end_;
    size_t                   number_not_used_;
    size_t                   data_not_used_;
    //
    pod_vector<pair_size_t> data_;
    pod_vector<size_t>      start_;
    pod_vector<size_t>      post_;
    pod_vector<size_t>      temporary_;
/* %$$
$end
------------------------------------------------------------------------------
$begin list_setvec_reference_count$$
$spell
    vec
    setvec
    const
$$

$section Return Number of References to a Set$$

$head Syntax$$
$icode%count% = %vec%.reference_count(%i%)%$$

$head vec$$
Is a $code list_setvec$$ object and can be $code const$$.

$head i$$
is the index of the set that we are retrieving the references for.

$head count$$
is the reference count.

$head Prototype$$
$srccode%hpp% */
private:
    size_t reference_count(size_t i) const
/* %$$
$end
*/
    {   // start data index
        size_t start = start_[i];
        if( start == 0 )
            return 0;
        //
        // reference count
        return data_[start].value;
    }
/*
------------------------------------------------------------------------------
$begin list_setvec_drop$$
$spell
    vec
    decremented
$$
$section Drop a Set That is No Longer Being Used$$

$head Syntax$$
$icode%not_used% = %vec%.drop(%i%)%$$

$head i$$
is the index of the set that is dropped.

$head reference_count$$
if the set is non-empty, the
$cref/reference count/list_setvec_member_data/start_/Reference Count/$$
is decremented.

$head start_$$
The value $codei%start_[%i%]%$$ is set to zero; i.e.,
the $th i$$ set is empty after the call.

$head post_$$
The value $codei%post_[%i%]%$$ is set to zero; i.e.,
any postings to the list are also dropped.

$head data_not_used_$$
The elements of $code data_$$ were information for the $th i$$ set,
and are no longer being used, are added to the linked list starting at
$code data_not_used$$.
This includes both set elements and postings.

$head not_used$$
is the number of elements of $code data_$$ that were begin used for the
$th i$$ set and are no longer being used; i.e., the number of elements
moved to $code data_not_used$$.

$head Prototype$$
$srccode%hpp% */
private:
    size_t drop(size_t i)
/* %$$
$end
*/
    {   // inialize count of addition elements not being used.
        size_t number_drop = 0;

        // the elements in the post list will no longer be used
        size_t post = post_[i];
        if( post != 0 )
        {   // drop this posting
            post_[i]    = 0;
            //
            // count elements in this posting
            ++number_drop;
            size_t previous = post;
            size_t next     = data_[previous].next;
            while( next != 0 )
            {   previous = next;
                next     = data_[previous].next;
                ++number_drop;
            }
            //
            // add the posting elements to data_not_used_
            data_[previous].next = data_not_used_;
            data_not_used_       = post;
        }

        // check for empty set
        size_t start = start_[i];
        if( start == 0 )
            return number_drop;

        // decrement reference counter
        CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );
        data_[start].value--;

        // set this set to empty
        start_[i] = 0;

        // If new reference count is positive, the list corresponding to
        // start is still being used.
        if( data_[start].value > 0 )
            return number_drop;

        //
        // count elements representing this set
        ++number_drop;
        size_t previous = start;
        size_t next     = data_[previous].next;
        while( next != 0 )
        {   previous = next;
            next     = data_[previous].next;
            ++number_drop;
        }
        //
        // add representing this set to data_not_used_
        data_[previous].next = data_not_used_;
        data_not_used_       = start;
        //
        return number_drop;
    }
/*
------------------------------------------------------------------------------
$begin list_setvec_get_data_index$$
$spell
    decremented
    setvec
    vec
$$

$section class list_setvec: Get a New List Pair$$

$head Syntax$$
$icode%index% = %vec%.get_data_index()%$$

$head vec$$
Is a $code list_setvec$$ object.

$head data_not_used_$$
If the input value of $code data_not_used_$$ is zero,
it is not changed.
Otherwise, the index for the element at the front of that list is returned.
In this case,
$code data_not_used$$ is advanced to the next element in that list.

$head number_not_used_$$
If the input value of $code data_not_used_$$ is zero,
$code number_not_used_$$ is not changed.
Otherwise it is decremented by one.

$head index$$
If the input value of $code data_not_used_$$ is zero,
the size of $code data_$$ is increased by one and index corresponding
to the end of $code data_$$ is returned.
Otherwise, the input value for $code data_not_used_$$ is returned.

$head Prototype$$
$srccode%hpp% */
private:
    size_t get_data_index(void)
/* %$$
$end
*/
    {   size_t index;
        if( data_not_used_ > 0 )
        {   CPPAD_ASSERT_UNKNOWN( number_not_used_ > 0 );
            --number_not_used_;
            index          = data_not_used_;
            data_not_used_ = data_[index].next;
        }
        else
        {   index = data_.extend(1);
        }
        return index;
    }
/*
-------------------------------------------------------------------------------
$begin list_setvec_check_data_structure$$
$spell
    setvec
    vec
    const
$$

$section class list_setvec: Check Data Structure$$

$head Syntax$$
$icode%vec%.check_data_structure()%$$

$head vec$$
Is a $code list_setvec$$ object that is effectively const.
It is not declared const because the data structure is modified and
then restored.

$head NDEBUG$$
If $code NDEBUG$$ is defined, the routine does nothing.
Otherwise, if an error is found in the data structure,
a $code CPPAD_ASSERT_UNKNOWN$$ is generated.

$head Prototype$$
$srccode%hpp% */
private:
    void check_data_structure(void)
/* %$$
$end
*/
# ifdef NDEBUG
    {   return; }
# else
    {   // number of sets
        CPPAD_ASSERT_UNKNOWN( post_.size() == start_.size() );
        size_t n_set = start_.size();
        if( n_set == 0 )
        {   CPPAD_ASSERT_UNKNOWN( end_ == 0 );
            CPPAD_ASSERT_UNKNOWN( number_not_used_ == 0 );
            CPPAD_ASSERT_UNKNOWN( data_not_used_ == 0 );
            CPPAD_ASSERT_UNKNOWN( data_.size() == 0 );
            CPPAD_ASSERT_UNKNOWN( start_.size() == 0 );
            return;
        }
        // check data index zero
        CPPAD_ASSERT_UNKNOWN( data_[0].value == end_ );
        CPPAD_ASSERT_UNKNOWN( data_[0].next  == 0  );
        // -----------------------------------------------------------
        // save a copy of the reference counters in temporary_
        temporary_.resize(n_set);
        for(size_t i = 0; i < n_set; i++)
            temporary_[i] = reference_count(i);
        // -----------------------------------------------------------
        // Initialize number of entries in data used by sets and posts.
        // Start with 1 for data_[0].
        size_t number_used_by_sets = 1;
        // -----------------------------------------------------------
        // count the number of entries in data_ that are used by sets
        for(size_t i = 0; i < n_set; i++)
        {   size_t start = start_[i];
            if( start > 0 )
            {   // check structure for this non-empty set
                size_t reference_count = data_[start].value;
                size_t next            = data_[start].next;
                CPPAD_ASSERT_UNKNOWN( reference_count > 0 );
                CPPAD_ASSERT_UNKNOWN( next != 0 );
                CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
                //
                // decrement the reference counter
                data_[start].value--;
                //
                // count the entries when find last reference
                if( data_[start].value == 0 )
                {
                    // restore reference count
                    data_[start].value = temporary_[i];

                    // number of data entries used for this set
                    number_used_by_sets += number_elements(i) + 1;
                    /*
                    number of elements checks that value < end_
                    .resizeeach pair in the list except for the start pair
                    and the pair with index zero.
                    */
                }
            }
        }
        // ------------------------------------------------------------------
        // count the number of entries in data_ that are used by posts
        size_t number_used_by_posts = 0;
        for(size_t i = 0; i < n_set; i++)
        {   size_t post = post_[i];
            if( post > 0 )
            {   size_t value = data_[post].value;
                size_t next  = data_[post].next;
                CPPAD_ASSERT_UNKNOWN( value < end_ );
                //
                while( value < end_ )
                {   ++number_used_by_posts;
                    value = data_[next].value;
                    next  = data_[next].next;
                }
            }
        }
        // ------------------------------------------------------------------
        // count number of entries in data_not_used_
        size_t count = 0;
        size_t next = data_not_used_;
        while( next != 0 )
        {   ++count;
            next = data_[next].next;
        }
        CPPAD_ASSERT_UNKNOWN( number_not_used_ == count );
        // ------------------------------------------------------------------
        size_t number_used = number_used_by_sets + number_used_by_posts;
        CPPAD_ASSERT_UNKNOWN(
            number_used + number_not_used_ == data_.size()
        );
        return;
    }
# endif
    // -----------------------------------------------------------------
    /*!
    Check if one of two sets is a subset of the other set

    \param one_this
    is the index in this sparse::svec_setvec object of the first set.

    \param two_other
    is the index in other sparse::svec_setvec object of the second set.

    \param other
    is the other sparse::svec_setvec object which may be the same as this object.

    \return
    If zero, niether set is a subset of the other.
    If one, then one is a subset of two and they are not equal.
    If two, then two is a subset of one and they are not equal.
    If three, then the sets are equal.
    */
    size_t is_subset(
        size_t                  one_this    ,
        size_t                  two_other   ,
        const list_setvec&      other       ) const
    {
        CPPAD_ASSERT_UNKNOWN( one_this  < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( two_other < other.start_.size()   );
        CPPAD_ASSERT_UNKNOWN( end_  == other.end_               );
        //
        // start
        size_t start_one    = start_[one_this];
        size_t start_two    = other.start_[two_other];
        //
        if( start_one == 0 )
        {   // set one is empty
            if( start_two == 0 )
            {   // set two is empty
                return 3;
            }
            return 1;
        }
        if( start_two == 0 )
        {   // set two is empty and one is not empty
            return 2;
        }
        //
        // next
        size_t next_one     = data_[start_one].next;
        size_t next_two     = other.data_[start_two].next;
        //
        // value
        size_t value_one    = data_[next_one].value;
        size_t value_two    = other.data_[next_two].value;
        //
        bool one_subset     = true;
        bool two_subset     = true;
        //
        size_t value_union = std::min(value_one, value_two);
        while( (one_subset | two_subset) & (value_union < end_) )
        {   if( value_one > value_union )
                two_subset = false;
            else
            {   next_one = data_[next_one].next;
                value_one = data_[next_one].value;
            }
            if( value_two > value_union )
                one_subset = false;
            else
            {   next_two = other.data_[next_two].next;
                value_two = other.data_[next_two].value;
            }
            value_union = std::min(value_one, value_two);
        }
        if( one_subset )
        {   if( two_subset )
            {   // sets are equal
                return 3;
            }
            // one is a subset of two
            return 1;
        }
        if( two_subset )
        {   // two is a subset of one
            return 2;
        }
        //
        // neither is a subset
        return 0;
    }
    // -----------------------------------------------------------------
    /*!
    Assign a set equal to the union of a set and a vector;

    \param target
    is the index in this list_setvec object of the set being assinged.

    \param left
    is the index in this list_setvec object of the
    left operand for the union operation.
    It is OK for target and left to be the same value.

    \param right
    is a vector of size_t, sorted in accending order.
    right operand for the union operation.
    Elements can be repeated in right, but are not be repeated in the
    resulting set.
    All of the elements must have value less than end();
    */
    void binary_union(
        size_t                    target ,
        size_t                    left   ,
        const pod_vector<size_t>& right  )
    {   CPPAD_ASSERT_UNKNOWN( post_[left] == 0 );
        //
        CPPAD_ASSERT_UNKNOWN( target < start_.size() );
        CPPAD_ASSERT_UNKNOWN( left   < start_.size() );

        // get start indices before drop modifies modify start_ in case target
        // and left are the same.
        size_t start_left   = start_[left];

        // -------------------------------------------------------------------
        // Check if right is a subset of left so that we used reference count
        // and not copies of identical sets.
        //
        // initialize index for left and right sets
        size_t current_left  = start_left;
        size_t current_right = 0;
        //
        // initialize value_left
        size_t value_left  = end_;
        if( current_left > 0 )
        {   // advance from reference counter to data
            current_left = data_[current_left].next;
            CPPAD_ASSERT_UNKNOWN( current_left != 0 )
            //
            value_left = data_[current_left].value;
            CPPAD_ASSERT_UNKNOWN( value_left < end_);
        }
        //
        // initialize value_right
        size_t value_right = end_;
        if( right.size() > 0 )
            value_right = right[current_right];
        //
        bool subset = true;
        while( subset & (value_right < end_) )
        {   while( value_left < value_right )
            {   // advance left
                current_left = data_[current_left].next;
                value_left = data_[current_left].value;
            }
            if( value_right < value_left )
                subset = false;
            else
            {   // advance right
                ++current_right;
                if( current_right == right.size() )
                    value_right = end_;
                else
                    value_right = right[current_right];
            }
        }
        //
        if( subset )
        {   // target = left will use reference count for identical sets
            assignment(target, left, *this);
            return;
        }

        // -------------------------------------------------------------------

        // start new version of target
        size_t start        = get_data_index();
        data_[start].value  = 1; // reference count
        //
        // previous index for new set
        size_t previous_target = start;
        //
        // initialize index for left and right sets
        current_left  = start_left;
        current_right = 0;
        //
        // initialize value_left
        value_left  = end_;
        if( current_left > 0 )
        {   // advance from reference counter to data
            current_left = data_[current_left].next;
            CPPAD_ASSERT_UNKNOWN( current_left != 0 )
            //
            value_left = data_[current_left].value;
            CPPAD_ASSERT_UNKNOWN( value_left < end_);
        }
        //
        // initialize value_right
        value_right = end_;
        if( right.size() > 0 )
            value_right = right[current_right];
        //
        // merge
        while( (value_left < end_) | (value_right < end_) )
        {   if( value_left == value_right)
            {   // advance left so left and right are no longer equal
                current_left = data_[current_left].next;
                value_left   = data_[current_left].value;
                CPPAD_ASSERT_UNKNOWN( value_right < value_left );
            }
            // place to put new element
            size_t current_target       = get_data_index();
            data_[previous_target].next = current_target;
            //
            if( value_left < value_right )
            {   // value_left case
                CPPAD_ASSERT_UNKNOWN( value_left < end_ );
                data_[current_target].value = value_left;
                //
                // advance left
                current_left = data_[current_left].next;
                value_left   = data_[current_left].value;
            }
            else
            {   CPPAD_ASSERT_UNKNOWN( value_right < value_left )
                // value_right case
                CPPAD_ASSERT_UNKNOWN( value_right < end_);
                data_[current_target].value = value_right;
                //
                // advance right (skip values equal to this one)
                size_t previous_value = value_right;
                while( value_right == previous_value )
                {   ++current_right;
                    if( current_right == right.size() )
                        value_right = end_;
                    else
                    {   value_right = right[current_right];
                        CPPAD_ASSERT_UNKNOWN( value_right < end_ );
                    }
                }
            }
            // done setting current target value
            previous_target  = current_target;
        }
        // make end of target list
        data_[previous_target].next = 0;

        // adjust number_not_used_
        size_t number_drop = drop(target);
        number_not_used_  += number_drop;

        // set the new start value for target
        start_[target] = start;

        return;
    }
// ===========================================================================
public:
    /// declare a const iterator
    typedef list_setvec_const_iterator const_iterator;
    // -----------------------------------------------------------------
    /*!
    Default constructor (no sets)
    */
    list_setvec(void) :
    end_(0)              ,
    number_not_used_(0)  ,
    data_not_used_(0)    ,
    data_(0)             ,
    start_(0)            ,
    post_(0)
    { }
    // -----------------------------------------------------------------
    /// Destructor
    ~list_setvec(void)
    {   check_data_structure();
    }
    // -----------------------------------------------------------------
    /*!
    Using copy constructor is a programing (not user) error

    \param v
    vector of sets that we are attempting to make a copy of.
    */
    list_setvec(const list_setvec& v)
    {   // Error: Probably a list_setvec argument has been passed by value
        CPPAD_ASSERT_UNKNOWN(false);
    }
    // -----------------------------------------------------------------
    /*!
    Assignement operator.

    \param other
    this list_setvec with be set to a deep copy of other.

    \par vector_of_sets
    This public member function is not yet part of
    the vector_of_sets concept.
    */
    void operator=(const list_setvec& other)
    {   end_             = other.end_;
        number_not_used_ = other.number_not_used_;
        data_not_used_   = other.data_not_used_;
        data_            = other.data_;
        start_           = other.start_;
        post_            = other.post_;
    }
    // -----------------------------------------------------------------
    /*!
    swap (used by move semantics version of ADFun assignment operator)

    \param other
    this list_setvec with be swapped with other.

    \par vector_of_sets
    This public member function is not yet part of
    the vector_of_sets concept.
    */
    void swap(list_setvec& other)
    {   // size_t objects
        std::swap(end_             , other.end_);
        std::swap(number_not_used_ , other.number_not_used_);
        std::swap(data_not_used_   , other.data_not_used_);

        // pod_vectors
        data_.swap(       other.data_);
        start_.swap(      other.start_);
        post_.swap(       other.post_);
        temporary_.swap(  other.temporary_);
    }
    // -----------------------------------------------------------------
    /*!
    Start a new vector of sets.

    \param n_set
    is the number of sets in this vector of sets.
    \li
    If n_set is zero, any memory currently allocated for this object
    is freed.
    \li
    If n_set is non-zero, a vector of n_set sets is created and all
    the sets are initilaized as empty.

    \param end
    is the maximum element plus one (the minimum element is 0).
    If n_set is zero, end must also be zero.
    */
    void resize(size_t n_set, size_t end)
    {   check_data_structure();

        if( n_set == 0 )
        {   CPPAD_ASSERT_UNKNOWN( end == 0 );
            //
            // restore object to start after constructor
            // (no memory allocated for this object)
            data_.clear();
            start_.clear();
            post_.clear();
            number_not_used_  = 0;
            data_not_used_    = 0;
            end_              = 0;
            //
            return;
        }
        end_                   = end;
        //
        start_.resize(n_set);
        post_.resize(n_set);
        //
        for(size_t i = 0; i < n_set; i++)
        {   start_[i] = 0;
            post_[i]  = 0;
        }
        //
        // last element, marks the end for all lists
        data_.resize(1);
        data_[0].value    = end_;
        data_[0].next     = 0;
        //
        number_not_used_  = 0;
        data_not_used_    = 0;
    }
    // -----------------------------------------------------------------
    /*!
    Count number of elements in a set.

    \param i
    is the index of the set we are counting the elements of.

    \par
    number of elements checks that value < end_ for each element of the set.
    */
    size_t number_elements(size_t i) const
    {   CPPAD_ASSERT_UNKNOWN( post_[i] == 0 );

        // check if the set is empty
        size_t start   = start_[i];
        if( start == 0 )
            return 0;

        // initialize counter
        size_t count   = 0;

        // advance to the first element in the set
        size_t next    = data_[start].next;
        while( next != 0 )
        {   CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
            count++;
            next  = data_[next].next;
        }
        CPPAD_ASSERT_UNKNOWN( count > 0 );
        return count;
    }
    /*!
    Post an element for delayed addition to a set.

    \param i
    is the index for this set in the vector of sets.

    \param element
    is the value of the element that we are posting.
    The same element may be posted multiple times.

    \par
    It is faster to post multiple elements to set i and then call
    process_post(i) then to add each element individually.
    It is an error to call any member function,
    that depends on the value of set i,
    before processing the posts to set i.
    */
    void post_element(size_t i, size_t element)
    {   CPPAD_ASSERT_UNKNOWN( i < start_.size() );
        CPPAD_ASSERT_UNKNOWN( element < end_ );

        // put element at the front of this list
        size_t next         = post_[i];
        size_t post         = get_data_index();
        post_[i]            = post;
        data_[post].value   = element;
        data_[post].next    = next;

        return;
    }
    // -----------------------------------------------------------------
    /*!
    process post entries for a specific set.

    \param i
    index of the set for which we are processing the post entries.

    \par post_
    Upon call, post_[i] is location in data_ of the elements that get
    added to the i-th set.  Upon return, post_[i] is zero.
    */
    void process_post(size_t i)
    {   // post
        size_t post = post_[i];
        //
        // check if there are no elements to process
        if( post == 0 )
            return;
        //
        // check if there is only one element to process
        size_t next  = data_[post].next;
        if( next == 0 )
        {   // done with this posting
            size_t value     = data_[post].value;
            post_[i]         = 0;
            data_[post].next = data_not_used_;
            data_not_used_   = post;
            ++number_not_used_;
            //
            add_element(i, value);
            //
            return;
        }
        //
        // copy the elements that need to be processed into temporary
        temporary_.resize(0);
        size_t previous  = post;
        size_t value     = data_[previous].value;
        CPPAD_ASSERT_UNKNOWN( value < end_ );
        temporary_.push_back(value);
        while( next != 0 )
        {   previous = next;
            value    = data_[previous].value;
            CPPAD_ASSERT_UNKNOWN( value < end_ );
            temporary_.push_back(value);
            next     = data_[previous].next;
        }
        size_t number_post = temporary_.size();
        //
        // done with this posting
        post_[i]              = 0;
        data_[previous].next  = data_not_used_;
        data_not_used_        = post;
        number_not_used_     += number_post;;
        //
        // sort temporary_
        CPPAD_ASSERT_UNKNOWN( number_post > 1 );
        std::sort( temporary_.data(), temporary_.data() + number_post);
        //
        // add the elements to the set
        binary_union(i, i, temporary_);
        //
        return;
    }
    // -----------------------------------------------------------------
    /*!
    Add one element to a set.

    \param i
    is the index for this set in the vector of sets.

    \param element
    is the element we are adding to the set.
    */
    void add_element(size_t i, size_t element)
    {   CPPAD_ASSERT_UNKNOWN( i   < start_.size() );
        CPPAD_ASSERT_UNKNOWN( element < end_ );

        // check for case where starting set is empty
        size_t start = start_[i];
        if( start == 0 )
        {   start              = get_data_index();
            start_[i]          = start;
            data_[start].value = 1; // reference count
            //
            size_t next        = get_data_index();
            data_[start].next  = next;
            //
            data_[next].value  = element;
            data_[next].next   = 0;
            return;
        }
        //
        // start of set with this index
        size_t previous = start_[i];
        //
        // first entry in this set
        size_t next     = data_[previous].next;
        size_t value    = data_[next].value;
        //
        // locate place to insert this element
        while( value < element )
        {   previous = next;
            next     = data_[next].next;
            value = data_[next].value;
        }
        //
        // check for case where element is in the set
        if( value == element )
            return;
        //
        //
        // check for case where this is the only reference to this set
        CPPAD_ASSERT_UNKNOWN( element < value );
        if( data_[start].value == 1 )
        {   size_t insert         = get_data_index();
            data_[insert].next    = next;
            data_[insert].value   = element;
            data_[previous].next  = insert;
            //
            return;
        }
        //
        // must make a separate copy with new element inserted
        CPPAD_ASSERT_UNKNOWN( data_[start].value > 1 );
        data_[start].value--;   // reverence counter for old list
        //
        size_t start_new       = get_data_index();
        data_[start_new].value = 1;         // reference counter for new list
        size_t previous_new    = start_new;
        //
        // start of old set with this index
        previous  = start_[i];
        //
        // first entry in old set
        next    = data_[previous].next;
        value   = data_[next].value;
        //
        // locate place to insert this element
        while( value < element )
        {   // copy to new list
            size_t next_new          = get_data_index();
            data_[previous_new].next = next_new;
            data_[next_new].value    = value;
            previous_new             = next_new;
            //
            // get next value
            previous = next;
            next     = data_[next].next;
            value = data_[next].value;
        }
        CPPAD_ASSERT_UNKNOWN( element < value );
        //
        // insert the element
        size_t next_new          = get_data_index();
        data_[previous_new].next = next_new;
        data_[next_new].value    = element;
        previous_new             = next_new;
        //
        // copy rest of the old set
        while( value < end_ )
        {   // copy to new list
            next_new                 = get_data_index();
            data_[previous_new].next = next_new;
            data_[next_new].value    = value;
            previous_new             = next_new;
            //
            // get next value
            previous = next;
            next     = data_[next].next;
            value = data_[next].value;
        }
        CPPAD_ASSERT_UNKNOWN( next == 0 );
        data_[previous_new].next = 0;
        //
        // hook up new list
        start_[i] = start_new;
        return;
    }
    // -----------------------------------------------------------------
    /*!
    check an element is in a set.

    \param i
    is the index for this set in the vector of sets.

    \param element
    is the element we are checking to see if it is in the set.
    */
    bool is_element(size_t i, size_t element) const
    {   CPPAD_ASSERT_UNKNOWN( post_[i] == 0 );
        CPPAD_ASSERT_UNKNOWN( element < end_ );
        //
        size_t start = start_[i];
        if( start == 0 )
            return false;
        //
        size_t next  = data_[start].next;
        size_t value = data_[next].value;
        while( value < element )
        {   next  = data_[next].next;
            value = data_[next].value;
        }
        return element == value;
    }
    // -----------------------------------------------------------------
    /*!
    Assign the empty set to one of the sets.

    \param target
    is the index of the set we are setting to the empty set.

    \par number_not_used_
    increments this value by additional number of data_ elements that are
    no longer being used.
    */
    void clear(size_t target)
    {   CPPAD_ASSERT_UNKNOWN( target < start_.size() );

        // adjust number_not_used_
        size_t number_drop = drop(target);
        number_not_used_  += number_drop;

        return;
    }
    // -----------------------------------------------------------------
    /*!
    Assign one set equal to another set.

    \param this_target
    is the index in this list_setvec object of the set being assinged.

    \param other_source
    is the index in the other list_setvec object of the
    set that we are using as the value to assign to the target set.

    \param other
    is the other list_setvec object (which may be the same as this
    list_setvec object). This must have the same value for end_.

    \par number_not_used_
    increments this value by additional number of elements not being used.
    */
    void assignment(
        size_t               this_target  ,
        size_t               other_source ,
        const list_setvec&   other        )
    {   CPPAD_ASSERT_UNKNOWN( other.post_[ other_source ] == 0 );
        //
        CPPAD_ASSERT_UNKNOWN( this_target  <   start_.size()        );
        CPPAD_ASSERT_UNKNOWN( other_source <   other.start_.size()  );
        CPPAD_ASSERT_UNKNOWN( end_        == other.end_   );

        // check if we are assigning a set to itself
        if( (this == &other) & (this_target == other_source) )
            return;

        // set depending on cases below
        size_t this_start;

        // If this and other are the same, use another reference to same list
        size_t other_start = other.start_[other_source];
        if( this == &other )
        {   this_start = other_start;
            if( other_start != 0 )
            {   data_[other_start].value++; // increment reference count
                CPPAD_ASSERT_UNKNOWN( data_[other_start].value > 1 );
            }
        }
        else if( other_start  == 0 )
        {   this_start = 0;
        }
        else
        {   // make a copy of the other list in this list_setvec
            this_start        = get_data_index();
            size_t this_next  = get_data_index();
            data_[this_start].value = 1; // reference count
            data_[this_start].next  = this_next;
            //
            size_t next  = other.data_[other_start].next;
            CPPAD_ASSERT_UNKNOWN( next != 0 );
            while( next != 0 )
            {   data_[this_next].value = other.data_[next].value;
                next                   = other.data_[next].next;
                if( next == 0 )
                    data_[this_next].next = 0;
                else
                {   size_t tmp = get_data_index();
                    data_[this_next].next = tmp;
                    this_next             = tmp;
                }
            }
        }

        // adjust number_not_used_
        size_t number_drop = drop(this_target);
        number_not_used_  += number_drop;

        // set the new start value for this_target
        start_[this_target] = this_start;

        return;
    }
    // -----------------------------------------------------------------
    /*!
    Assign a set equal to the union of two other sets.

    \param this_target
    is the index in this list_setvec object of the set being assinged.

    \param this_left
    is the index in this list_setvec object of the
    left operand for the union operation.
    It is OK for this_target and this_left to be the same value.

    \param other_right
    is the index in the other list_setvec object of the
    right operand for the union operation.
    It is OK for this_target and other_right to be the same value.

    \param other
    is the other list_setvec object (which may be the same as this
    list_setvec object).
    */
    void binary_union(
        size_t                  this_target  ,
        size_t                  this_left    ,
        size_t                  other_right  ,
        const list_setvec&      other        )
    {   CPPAD_ASSERT_UNKNOWN( post_[this_left] == 0 );
        CPPAD_ASSERT_UNKNOWN( other.post_[ other_right ] == 0 );
        //
        CPPAD_ASSERT_UNKNOWN( this_target < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( this_left   < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( other_right < other.start_.size()   );
        CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );

        // check if one of the two operands is a subset of the the other
        size_t subset = is_subset(this_left, other_right, other);

        // case where right is a subset of left or right and left are equal
        if( subset == 2 || subset == 3 )
        {   assignment(this_target, this_left, *this);
            return;
        }
        // case where the left is a subset of right and they are not equal
        if( subset == 1 )
        {   assignment(this_target, other_right, other);
            return;
        }
        // if niether case holds, then both left and right are non-empty
        CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
        CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

        // must get all the start indices before modify start_this
        // (incase start_this is the same as start_left or start_right)
        size_t start_left    = start_[this_left];
        size_t start_right   = other.start_[other_right];

        // start the new list
        size_t start        = get_data_index();
        size_t next         = start;
        data_[start].value  = 1; // reference count

        // next for left and right lists
        size_t next_left   = data_[start_left].next;
        size_t next_right  = other.data_[start_right].next;

        // value for left and right sets
        size_t value_left  = data_[next_left].value;
        size_t value_right = other.data_[next_right].value;

        CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );
        while( (value_left < end_) | (value_right < end_) )
        {   if( value_left == value_right )
            {   // advance right so left and right are no longer equal
                next_right  = other.data_[next_right].next;
                value_right = other.data_[next_right].value;
            }
            if( value_left < value_right )
            {   size_t tmp        = get_data_index();
                data_[next].next  = tmp;
                next              = tmp;
                data_[next].value = value_left;
                // advance left to its next element
                next_left  = data_[next_left].next;
                value_left = data_[next_left].value;
            }
            else
            {   CPPAD_ASSERT_UNKNOWN( value_right < value_left )
                size_t tmp        = get_data_index();
                data_[next].next  = tmp;
                next              = tmp;
                data_[next].value = value_right;
                // advance right to its next element
                next_right  = other.data_[next_right].next;
                value_right = other.data_[next_right].value;
            }
        }
        data_[next].next = 0;

        // adjust number_not_used_
        size_t number_drop = drop(this_target);
        number_not_used_  += number_drop;

        // set the new start value for this_target
        start_[this_target] = start;

        return;
    }
    // -----------------------------------------------------------------
    /*!
    Assign a set equal to the intersection of two other sets.

    \param this_target
    is the index in this list_setvec object of the set being assinged.

    \param this_left
    is the index in this list_setvec object of the
    left operand for the intersection operation.
    It is OK for this_target and this_left to be the same value.

    \param other_right
    is the index in the other list_setvec object of the
    right operand for the intersection operation.
    It is OK for this_target and other_right to be the same value.

    \param other
    is the other list_setvec object (which may be the same as this
    list_setvec object).
    */
    void binary_intersection(
        size_t                  this_target  ,
        size_t                  this_left    ,
        size_t                  other_right  ,
        const list_setvec&      other        )
    {   CPPAD_ASSERT_UNKNOWN( post_[this_left] == 0 );
        CPPAD_ASSERT_UNKNOWN( other.post_[ other_right ] == 0 );
        //
        CPPAD_ASSERT_UNKNOWN( this_target < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( this_left   < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( other_right < other.start_.size()   );
        CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );
        //
        // check if one of the two operands is a subset of the the other
        size_t subset = is_subset(this_left, other_right, other);

        // case where left is a subset of right or left and right are equal
        if( subset == 1 || subset == 3 )
        {   assignment(this_target, this_left, *this);
            return;
        }
        // case where the right is a subset of left and they are not equal
        if( subset == 2 )
        {   assignment(this_target, other_right, other);
            return;
        }
        // if niether case holds, then both left and right are non-empty
        CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
        CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

        // must get all the start indices before modify start_this
        // (incase start_this is the same as start_left or start_right)
        size_t start_left    = start_[this_left];
        size_t start_right   = other.start_[other_right];

        // start the new list as emptyh
        size_t start        = 0;
        size_t next         = start;

        // next for left and right lists
        size_t next_left   = data_[start_left].next;
        size_t next_right  = other.data_[start_right].next;

        // value for left and right sets
        size_t value_left  = data_[next_left].value;
        size_t value_right = other.data_[next_right].value;

        CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );
        while( (value_left < end_) & (value_right < end_) )
        {   if( value_left == value_right )
            {   if( start == 0 )
                {   // this is the first element in the intersection
                    start               = get_data_index();
                    next                = start;
                    data_[start].value  = 1; // reference count
                    CPPAD_ASSERT_UNKNOWN( start > 0 );
                    // must delay following assignment until after drop below
                    // start_[this_target] = start;
                }
                size_t tmp        = get_data_index();
                data_[next].next  = tmp;
                next              = tmp;
                data_[next].value = value_left;
                //
                // advance left
                next_left  = data_[next_left].next;
                value_left = data_[next_left].value;
                //
            }
            if( value_left > value_right )
            {   // advance right
                next_right  = other.data_[next_right].next;
                value_right = other.data_[next_right].value;
            }
            if( value_right > value_left )
            {   // advance left
                next_left  = data_[next_left].next;
                value_left = data_[next_left].value;
            }
        }
        if( start != 0 )
        {   CPPAD_ASSERT_UNKNOWN( next != 0 );
            data_[next].next = 0;
        }

        // adjust number_not_used_
        size_t number_drop = drop(this_target);
        number_not_used_  += number_drop;

        // set new start for this_target
        start_[this_target] = start;

        return;
    }
    // -----------------------------------------------------------------
    /*! Fetch n_set for vector of sets object.

    \return
    Number of from sets for this vector of sets object
    */
    size_t n_set(void) const
    {   return start_.size(); }
    // -----------------------------------------------------------------
    /*! Fetch end for this vector of sets object.

    \return
    is the maximum element value plus one (the minimum element value is 0).
    */
    size_t end(void) const
    {   return end_; }
    // -----------------------------------------------------------------
    /*! Amount of memory used by this vector of sets

    \return
    The amount of memory in units of type unsigned char memory.
    */
    size_t memory(void) const
    {   return data_.capacity() * sizeof(pair_size_t);
    }
    /*!
    Print the vector of sets (used for debugging)
    */
    void print(void) const;
};
// =========================================================================
/*!
cons_iterator for one set of positive integers in a list_setvec object.

All the public members for this class are also in the
sparse::pack_setvec_const_iterator and sparse::svec_setvec_const_iterator classes.
This defines the CppAD vector_of_sets iterator concept.
*/
class list_setvec_const_iterator {
private:
    /// type used by list_setvec to represent one element of the list
    typedef list_setvec::pair_size_t pair_size_t;

    /// data for the entire vector of sets
    const pod_vector<pair_size_t>& data_;

    /// Possible elements in a list are 0, 1, ..., end_ - 1;
    const size_t                   end_;

    /// next element in the singly linked list
    /// (next_pair_.value == end_ for past end of list)
    pair_size_t                    next_pair_;
public:
    /// construct a const_iterator for a list in a list_setvec object
    list_setvec_const_iterator (const list_setvec& list, size_t i)
    :
    data_( list.data_ )    ,
    end_ ( list.end_ )
    {   CPPAD_ASSERT_UNKNOWN( list.post_[i] == 0 );
        //
        size_t start = list.start_[i];
        if( start == 0 )
        {   next_pair_.next  = 0;
            next_pair_.value = end_;
        }
        else
        {   // value for this entry is reference count for list
            CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );

            // data index where list truely starts
            size_t next = data_[start].next;
            CPPAD_ASSERT_UNKNOWN( next != 0 );

            // true first entry in the list
            next_pair_ = data_[next];
            CPPAD_ASSERT_UNKNOWN( next_pair_.value < end_ );
        }
    }

    /// advance to next element in this list
    list_setvec_const_iterator& operator++(void)
    {   next_pair_  = data_[next_pair_.next];
        return *this;
    }

    /// obtain value of this element of the set of positive integers
    /// (end_ for no such element)
    size_t operator*(void)
    {   return next_pair_.value; }
};
// =========================================================================
/*!
Print the vector of sets (used for debugging)
*/
inline void list_setvec::print(void) const
{   std::cout << "list_setvec:\n";
    for(size_t i = 0; i < n_set(); i++)
    {   std::cout << "set[" << i << "] = {";
        const_iterator itr(*this, i);
        while( *itr != end() )
        {   std::cout << *itr;
            if( *(++itr) != end() )
                std::cout << ",";
        }
        std::cout << "}\n";
    }
    return;
}

/*!
Copy a user vector of sets sparsity pattern to an internal list_setvec object.

\tparam SetVector
is a simple vector with elements of type std::set<size_t>.

\param internal
The input value of sparisty does not matter.
Upon return it contains the same sparsity pattern as user
(or the transposed sparsity pattern).

\param user
sparsity pattern that we are placing internal.

\param n_set
number of sets (rows) in the internal sparsity pattern.

\param end
end of set value (number of columns) in the interanl sparsity pattern.

\param transpose
if true, the user sparsity patter is the transposed.

\param error_msg
is the error message to display if some values in the user sparstiy
pattern are not valid.
*/
template<class SetVector>
void sparsity_user2internal(
    list_setvec&            internal  ,
    const SetVector&        user      ,
    size_t                  n_set     ,
    size_t                  end       ,
    bool                    transpose ,
    const char*             error_msg )
{
# ifndef NDEBUG
    if( transpose )
        CPPAD_ASSERT_KNOWN( end == size_t( user.size() ), error_msg);
    if( ! transpose )
        CPPAD_ASSERT_KNOWN( n_set == size_t( user.size() ), error_msg);
# endif

    // iterator for user set
    std::set<size_t>::const_iterator itr;

    // size of internal sparsity pattern
    internal.resize(n_set, end);

    if( transpose )
    {   // transposed pattern case
        for(size_t j = 0; j < end; j++)
        {   itr = user[j].begin();
            while(itr != user[j].end())
            {   size_t i = *itr++;
                CPPAD_ASSERT_KNOWN(i < n_set, error_msg);
                internal.add_element(i, j);
            }
        }
    }
    else
    {   for(size_t i = 0; i < n_set; i++)
        {   itr = user[i].begin();
            while(itr != user[i].end())
            {   size_t j = *itr++;
                CPPAD_ASSERT_KNOWN( j < end, error_msg);
                internal.add_element(i, j);
            }
        }
    }
    return;
}

} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE

// =========================================================================
// Tell pod_vector class that each pair_size_t is plain old data and hence
// the corresponding constructor need not be called.
namespace CppAD { namespace local {
    template <> inline bool
    is_pod<sparse::list_setvec::pair_size_t>(void)
    {   return true; }
} }

# endif
