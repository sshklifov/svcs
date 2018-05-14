#include "ForwardListConstIterator.h"

template <typename T>
ForwardListConstIterator<T>::ForwardListConstIterator (std::nullptr_t) : it (nullptr)
{
}

template <typename T>
ForwardListConstIterator<T>::ForwardListConstIterator (const ForwardListNode<T>* node) : it (node)
{
}

template <typename T>
ForwardListConstIterator<T>::ForwardListConstIterator (ForwardListIterator<T> rhs) : it (rhs.it)
{
}

template <typename T>
const T& ForwardListConstIterator<T>::operator* () const
{
    return it->val;
}

template <typename T>
const T* ForwardListConstIterator<T>::operator-> () const
{
    return &(operator* ());
}

template <typename T>
ForwardListConstIterator<T>& ForwardListConstIterator<T>::operator++ ()
{
    it = it->link;
    return (*this);
}

template <typename T>
ForwardListConstIterator<T> ForwardListConstIterator<T>::operator++ (int)
{
    ForwardListConstIterator res (*this);
    ++(*this);
    return res;
}
