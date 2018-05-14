#ifndef FORWARD_LIST_ITERATOR_INCLUDED
#define FORWARD_LIST_ITERATOR_INCLUDED

#include <iterator>

template <typename T>
class ForwardList;

template <typename T>
class ForwardListConstIterator;
#include "ForwardList.h"

template <typename T>
class ForwardListIterator : public std::iterator<std::forward_iterator_tag, T>
{
    friend class ForwardList<T>;
    friend class ForwardListConstIterator<T>;

public:
    ForwardListIterator () = default;
    ForwardListIterator (std::nullptr_t);

    ForwardListIterator (const ForwardListIterator& rhs) = default;
    ForwardListIterator& operator= (const ForwardListIterator& rhs) = default;

    T& operator* ();
    const T& operator* () const;
    T* operator-> ();
    const T* operator-> () const;

    ForwardListIterator& operator++ ();
    ForwardListIterator operator++ (int);

private:
    ForwardListIterator (ForwardListNode<T>* node);

private:
    ForwardListNode<T>* it;
};

#include "ForwardListIterOps.h"
#include "ForwardListIterator.inl"

#endif /* FORWARD_LIST_ITERATOR_INCLUDED */
