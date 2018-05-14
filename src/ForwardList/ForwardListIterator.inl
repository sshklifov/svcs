#include "ForwardListIterator.h"

template <typename T>
ForwardListIterator<T>::ForwardListIterator (std::nullptr_t) : it (nullptr)
{
}

template <typename T>
ForwardListIterator<T>::ForwardListIterator (ForwardListNode<T>* node) : it (node)
{
}

template <typename T>
T& ForwardListIterator<T>::operator* ()
{
    return it->val;
}

template <typename T>
const T& ForwardListIterator<T>::operator* () const
{
    return it->val;
}

template <typename T>
T* ForwardListIterator<T>::operator-> ()
{
    return &(operator* ());
}

template <typename T>
const T* ForwardListIterator<T>::operator-> () const
{
    return &(operator* ());
}

template <typename T>
ForwardListIterator<T>& ForwardListIterator<T>::operator++ ()
{
    it = it->link;
    return (*this);
}

template <typename T>
ForwardListIterator<T> ForwardListIterator<T>::operator++ (int)
{
    ForwardListIterator res (*this);
    ++(*this);
    return res;
}
