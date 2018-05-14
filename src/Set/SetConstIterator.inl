#include "SetConstIterator.h"

template <typename T>
SetConstIterator<T>::SetConstIterator (InternalIterator it) : it (it)
{
}

template <typename T>
const T& SetConstIterator<T>::operator* () const
{
    return it->elem;
}

template <typename T>
const T* SetConstIterator<T>::operator-> () const
{
    return &(operator * ());
}

template <typename T>
SetConstIterator<T>& SetConstIterator<T>::operator++ ()
{
    ++it;
    return (*this);
}

template <typename T>
SetConstIterator<T> SetConstIterator<T>::operator++ (int)
{
    SetConstIterator res (*this);
    ++(*this);
    return res;
}

template <typename T>
bool operator!= (SetConstIterator<T> lhs, SetConstIterator<T> rhs)
{
    return !(lhs==rhs);
}
