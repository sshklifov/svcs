#include "ForwardList.h"

#include <Construct.h>
#include <TemplatedMalloc.h>

#include <utility>
#include <cassert>
#include <cstdlib>

template <typename T>
ForwardList<T>::ForwardList () : size (0)
{
    sentinel = TemplatedMalloc<ForwardListNode<T>> ();
    sentinel->link = NULL;
}

template <typename T>
ForwardList<T>::ForwardList (std::initializer_list<T> init) :
    ForwardList (init.begin (), init.begin () + init.size ())
{
}

template <typename T>
template <typename ForwardIt>
ForwardList<T>::ForwardList (ForwardIt first, ForwardIt last) : sentinel (NULL), size (0)
{
    Assign (first, last);
}

template <typename T>
ForwardList<T>::ForwardList (const ForwardList& rhs) : ForwardList (rhs.CBegin (), rhs.CEnd ())
{
}
template <typename T>
ForwardList<T>::ForwardList (ForwardList&& rhs) : sentinel (NULL), size (0)
{
    Swap (rhs);
}

template <typename T>
ForwardList<T>& ForwardList<T>::operator= (const ForwardList& rhs)
{
    if (this != &rhs)
    {
        Assign (rhs.CBegin (), rhs.CEnd ());
    }

    return (*this);
}

template <typename T>
ForwardList<T>& ForwardList<T>::operator= (ForwardList&& rhs)
{
    Swap (rhs);
    return (*this);
}

template <typename T>
ForwardList<T>::~ForwardList ()
{
    if (sentinel) Clear ();
    free (sentinel);
}

template <typename T>
typename ForwardList<T>::Iterator ForwardList<T>::BeforeBegin ()
{
    return Iterator (sentinel);
}

template <typename T>
typename ForwardList<T>::Iterator ForwardList<T>::Begin ()
{
    return Iterator (sentinel->link);
}

template <typename T>
typename ForwardList<T>::Iterator ForwardList<T>::End ()
{
    return Iterator (nullptr);
}

template <typename T>
typename ForwardList<T>::ConstIterator ForwardList<T>::CBeforeBegin () const
{
    return ConstIterator (sentinel);
}

template <typename T>
typename ForwardList<T>::ConstIterator ForwardList<T>::CBegin () const
{
    return ConstIterator (sentinel->link);
}

template <typename T>
typename ForwardList<T>::ConstIterator ForwardList<T>::CEnd () const
{
    return ConstIterator (nullptr);
}

template <typename T>
T& ForwardList<T>::Front ()
{
    assert (!Empty ());
    return *Begin ();
}

template <typename T>
const T& ForwardList<T>::Front () const
{
    assert (!Empty ());
    return *CBegin ();
}

template <typename T>
bool ForwardList<T>::Empty () const
{
    return Size () == 0;
}

template <typename T>
size_t ForwardList<T>::Size () const
{
    return size;
}

template <typename T>
template <typename... Args>
void ForwardList<T>::InsertFront (Args&&... args)
{
    InsertAfter (BeforeBegin (), std::forward<Args> (args)...);
}

template <typename T>
template <typename... Args>
void ForwardList<T>::InsertAfter (ConstIterator it, Args&&... args)
{
    ForwardListNode<T>* pBeforeInserted = const_cast<ForwardListNode<T>*> (it.it);
    ForwardListNode<T>* pAfterInserted = pBeforeInserted->link;
    ForwardListNode<T>* pInserted = TemplatedMalloc<ForwardListNode<T>> ();

    Construct (&pInserted->val, std::forward<Args> (args)...);
    pBeforeInserted->link = pInserted;
    pInserted->link = pAfterInserted;

    ++size;
}

template <typename T>
template <typename... Args>
void ForwardList<T>::InsertSentinel (Args&&... args)
{
    ForwardListNode<T>* newSentinel = TemplatedMalloc <ForwardListNode<T>> ();
    newSentinel->link = sentinel;
    sentinel = newSentinel;

    Construct (&sentinel->link->val, std::forward<Args> (args)...);
    ++size;
}

template <typename T>
void ForwardList<T>::RemoveFront ()
{
    assert (!Empty ());
    RemoveAfter (BeforeBegin ());
}

template <typename T>
void ForwardList<T>::RemoveAfter (ConstIterator it)
{
    assert (!Empty ());

    ForwardListNode<T>* pBeforeRemoved = const_cast<ForwardListNode<T>*> (it.it);
    ForwardListNode<T>* pRemoved = pBeforeRemoved->link;
    ForwardListNode<T>* pAfterRemoved = pRemoved->link;

    pBeforeRemoved->link = pAfterRemoved;
    Destroy (&pRemoved->val);
    free (pRemoved);
    
    --size;
}

template <typename T>
template <typename ForwardIt>
void ForwardList<T>::Assign (ForwardIt first, ForwardIt last)
{
    ForwardList tmp;
    Iterator head = tmp.BeforeBegin ();

    while (first != last)
    {
        tmp.InsertAfter (head, *first);
        ++head;
        ++first;
    }

    Swap (tmp);
}

template <typename T>
void ForwardList<T>::Clear ()
{
    while (!Empty ()) RemoveFront ();
}

template <typename T>
void ForwardList<T>::Swap (ForwardList& rhs)
{
    std::swap (sentinel, rhs.sentinel);
    std::swap (size, rhs.size);
}

