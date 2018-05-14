#ifndef FORWARD_LIST_CONST_ITERATOR_INCLUDED
#define FORWARD_LIST_CONST_ITERATOR_INCLUDED

#include <iterator>

template <typename T>
class ForwardList;

template <typename T>
class ForwardListIterator;

#include "ForwardList.h"

template <typename T>
class ForwardListConstIterator : public std::iterator<std::forward_iterator_tag, T>
{
    friend class ForwardList<T>;
    
    friend bool operator== (ForwardListConstIterator<T> lhs, ForwardListConstIterator<T> rhs)
    {
        return lhs.it == rhs.it;
    }

public:
    ForwardListConstIterator () = default;
    ForwardListConstIterator (std::nullptr_t);
    ForwardListConstIterator (ForwardListIterator<T> it);

    ForwardListConstIterator (const ForwardListConstIterator& rhs) = default;
    ForwardListConstIterator& operator= (const ForwardListConstIterator& rhs) = default;

    const T& operator* () const;
    const T* operator-> () const;

    ForwardListConstIterator& operator++ ();
    ForwardListConstIterator operator++ (int);

private:
    ForwardListConstIterator (const ForwardListNode<T>* node);

private:
    const ForwardListNode<T>* it;
};

#include "ForwardListIterOps.h"
#include "ForwardListConstIterator.inl"

#endif /* FORWARD_LIST_ITERATOR_INCLUDED */
