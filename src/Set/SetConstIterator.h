#ifndef SET_CONST_ITERATOR_INCLUDED
#define SET_CONST_ITERATOR_INCLUDED

#include <iterator>

template <typename T, typename Surject>
class Set;

#include "Set.h"

template <typename T>
class SetConstIterator : public std::iterator<std::forward_iterator_tag, T>
{
    template <typename S, typename Surject>
    friend class Set;

    friend bool operator== (SetConstIterator<T> lhs, SetConstIterator<T> rhs)
    {
        return lhs.it == rhs.it;
    }

public:
    SetConstIterator () = default;

    SetConstIterator (const SetConstIterator& rhs) = default;
    SetConstIterator& operator= (const SetConstIterator& rhs) = default;

    const T& operator* () const;
    const T* operator-> () const;

    SetConstIterator& operator++ ();
    SetConstIterator operator++ (int);

private:
    using InternalIterator = typename ForwardList<SetNode<T>>::Iterator;
    SetConstIterator (InternalIterator node);

private:
    InternalIterator it;
};

template <typename T>
bool operator!= (SetConstIterator<T> lhs, SetConstIterator<T> rhs);

#include "SetConstIterator.inl"

#endif /* SET_CONST_ITERATOR_INCLUDED */
