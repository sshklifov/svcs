#ifndef FORWARD_LIST_ITER_OPS_INCLUDED
#define FORWARD_LIST_ITER_OPS_INCLUDED

template <typename T>
class ForwardListIterator;
#include "ForwardListIterator.h"

template <typename T>
class ForwardListConstIterator;
#include "ForwardListConstIterator.h"

template <typename T>
bool operator== (ForwardListConstIterator<T> lhs, ForwardListIterator<T> rhs)
{
    return lhs == ForwardListConstIterator<T> (rhs);
}

template <typename T>
bool operator== (ForwardListIterator<T> lhs, ForwardListConstIterator<T> rhs)
{
    return rhs == lhs;
}

template <typename T>
bool operator== (ForwardListIterator<T> lhs, ForwardListIterator<T> rhs)
{
    return ForwardListConstIterator<T> (lhs) == ForwardListConstIterator<T> (rhs);
}

template <typename T>
bool operator== (ForwardListConstIterator<T> lhs, std::nullptr_t)
{
    return lhs == ForwardListConstIterator<T> (nullptr);
}

template <typename T>
bool operator== (ForwardListIterator<T> lhs, std::nullptr_t)
{
    return ForwardListConstIterator<T> (lhs) == ForwardListConstIterator<T> (nullptr);
}

template <typename T>
bool operator!= (ForwardListConstIterator<T> lhs, ForwardListConstIterator<T> rhs)
{
    return !(lhs==rhs);
}

template <typename T>
bool operator!= (ForwardListConstIterator<T> lhs, ForwardListIterator<T> rhs)
{
    return !(lhs==rhs);
}

template <typename T>
bool operator!= (ForwardListIterator<T> lhs, ForwardListConstIterator<T> rhs)
{
    return !(lhs==rhs);
}

template <typename T>
bool operator!= (ForwardListIterator<T> lhs, ForwardListIterator<T> rhs)
{
    return !(lhs==rhs);
}

template <typename T>
bool operator!= (ForwardListConstIterator<T> lhs, std::nullptr_t)
{
    return !(lhs==nullptr);
}

template <typename T>
bool operator!= (ForwardListIterator<T> lhs, std::nullptr_t)
{
    return !(lhs==nullptr);
}

#endif /* FORWARD_LIST_ITER_OPS_INCLUDED */
