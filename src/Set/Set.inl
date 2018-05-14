#include "Set.h"

#include <Construct.h>
#include <TemplatedMalloc.h>
#include <stdexcept>

template <typename T, typename Surject>
Set<T,Surject>::Set () : buckets (NULL), numBuckets (0)
{
}

template <typename T, typename Surject>
Set<T,Surject>::Set (Surject surject) : buckets (NULL), numBuckets (0), surject (std::move (surject))
{
}

template <typename T, typename Surject>
Set<T,Surject>::Set (size_t hint) : Set ()
{
    numBuckets = OptimalBucketCnt (hint);
    buckets = TemplatedMalloc<InternalIterator> (numBuckets);
    for (size_t i = 0; i < numBuckets; ++i) Construct (buckets + i, nullptr);
}

template <typename T, typename Surject>
Set<T,Surject>::Set (const Set& rhs) : Set ()
{
    CopyAssign (rhs);
}

template <typename T, typename Surject>
Set<T,Surject>::Set (Set&& rhs) : Set ()
{
    Swap (rhs);
}

template <typename T, typename Surject>
Set<T,Surject>& Set<T,Surject>::operator= (const Set& rhs)
{
    if (this != &rhs)
    {
        CopyAssign (rhs);
    }

    return (*this);
}

template <typename T, typename Surject>
Set<T,Surject>& Set<T,Surject>::operator= (Set&& rhs)
{
    Swap (rhs);
    return (*this);
}

template <typename T, typename Surject>
Set<T,Surject>::~Set ()
{
    free (buckets);
}

template <typename T, typename Surject>
void Set<T,Surject>::CopyAssign (const Set& rhs)
{
    Set tmp (rhs.Size ());
    for (InternalConstIterator it = rhs.nodes.CBegin (); it != rhs.nodes.CEnd (); ++it)
    {
        tmp.InsertNode (it->cachedHash, it->elem);
    }
    Swap (tmp);
}

template <typename T, typename Surject>
template <typename... Args>
void Set<T,Surject>::InsertNode (size_t cachedHash, Args&&... args)
{
    T elem (std::forward<Args> (args)...);
    if (Find (elem) != CEnd ())
    {
        throw std::logic_error ("Set::InsertNode: inserting duplicate element");
    }

    BalanceLoadFactor ();

    size_t bucketIdx = cachedHash % numBuckets;
    if (buckets[bucketIdx] != nullptr)
    {
        nodes.InsertAfter (buckets[bucketIdx], cachedHash, std::move (elem));
    }
    else
    {
        nodes.InsertSentinel (cachedHash, std::move (elem));
        buckets[bucketIdx] = nodes.BeforeBegin ();
    }
}

template <typename T, typename Surject>
template <typename... Args>
void Set<T,Surject>::Insert (Args&&... args)
{
    T elem (std::forward<Args> (args)...);
    size_t cachedHash = surject (elem);
    InsertNode (cachedHash, std::move (elem));
}

template <typename T, typename Surject>
template <typename... Args>
void Set<T,Surject>::Join (Args&&... args)
{
    T elem (std::forward<Args> (args)...);
    if (Find (elem) != CEnd ()) return;

    size_t cachedHash = surject (elem);
    InsertNode (cachedHash, std::move (elem));
}

template <typename T, typename Surject>
void Set<T,Surject>::Subtract (const T& elem)
{
    ConstIterator it = FindBefore (elem);
    if (it != CEnd ())
    {
        RemoveAfter (it);
    }
}

template <typename T, typename Surject>
void Set<T,Surject>::Remove (const T& elem)
{
    ConstIterator it = FindBefore (elem);
    if (it == CEnd ())
    {
        throw std::logic_error ("Set::Remove: element not found");
    }

    RemoveAfter (it);
}

template <typename T, typename Surject>
void Set<T,Surject>::RemoveAfter (ConstIterator it)
{
    assert (!Empty ());

    InternalIterator before = it.it;
    size_t bucketIdx = std::next(before)->cachedHash % numBuckets;

    nodes.RemoveAfter (before);
    if (std::next(buckets[bucketIdx]) == nodes.End () ||
            std::next(buckets[bucketIdx])->cachedHash % numBuckets != bucketIdx)
    {
        buckets[bucketIdx] = nullptr;
    }

    size_t nextBucketIdx;
    if (std::next (before) != nodes.CEnd () &&
            (nextBucketIdx=std::next(before)->cachedHash % numBuckets) != bucketIdx)
    {
        buckets[nextBucketIdx] = before;
    }
}

template <typename T, typename Surject>
void Set<T,Surject>::Remove (ConstIterator it)
{
    assert (!Empty ());

    InternalIterator curr = it.it;
    size_t cachedHash = curr->cachedHash;
    size_t bucketIdx = cachedHash % numBuckets;
    InternalIterator before = buckets[bucketIdx];

    assert (before != nullptr && before != nodes.CEnd ());
    while (std::next (before) != curr)
    {
        ++before;
        assert (std::next (before) != nodes.CEnd ());
        assert (before->cachedHash % numBuckets == bucketIdx);
    }

    RemoveAfter (ConstIterator (before));
}

template <typename T, typename Surject>
T Set<T,Surject>::Release (ConstIterator it)
{
    T res = std::move (it.it->elem);
    Remove (it);
    return res;
}

template <typename T, typename Surject>
typename Set<T,Surject>::ConstIterator Set<T,Surject>::FindBefore (const T& elem) const
{
    if (Empty ()) return CEnd ();

    size_t cachedHash = surject (elem);
    size_t bucketIdx = cachedHash % numBuckets;
    if (buckets[bucketIdx] != nullptr)
    {
        InternalIterator before = buckets[bucketIdx];
        InternalIterator curr = std::next (before);

        while (curr != nodes.CEnd () && curr->cachedHash % numBuckets == bucketIdx)
        {
            if (curr->cachedHash == cachedHash && elem == curr->elem)
            {
                return ConstIterator (before);
            }
            ++before;
            ++curr;
        }
    }

    return CEnd ();
}

template <typename T, typename Surject>
typename Set<T,Surject>::ConstIterator Set<T,Surject>::Find (const T& elem) const
{
    ConstIterator res = FindBefore (elem);
    return res==CEnd () ? res : ++res;
}

template <typename T, typename Surject>
template <typename Callable>
void Set<T,Surject>::ForEachInvImg (size_t image, Callable f) const
{
    if (Empty ()) return;

    size_t bucketIdx = image % numBuckets;
    if (buckets[bucketIdx] != nullptr)
    {
        InternalIterator it = std::next (buckets[bucketIdx]);
        while (it != nodes.CEnd () && it->cachedHash % numBuckets == bucketIdx)
        {
            if (it->cachedHash == image) f (ConstIterator (it));
            ++it;
        }
    }
}

template <typename T, typename Surject>
bool Set<T,Surject>::Empty () const
{
    return nodes.Empty ();
}

template <typename T, typename Surject>
size_t Set<T,Surject>::Size () const
{
    return nodes.Size ();
}

template <typename T, typename Surject>
void Set<T,Surject>::Clear ()
{
    nodes.Clear ();
    free (buckets);

    buckets = NULL;
    numBuckets = 0;
}

template <typename T, typename Surject>
void Set<T,Surject>::Swap (Set& rhs)
{
    nodes.Swap (rhs.nodes);
    std::swap (buckets, rhs.buckets);
    std::swap (numBuckets, rhs.numBuckets);
}

template <typename T, typename Surject>
const Surject& Set<T,Surject>::GetSurject () const
{
    return surject;
}

template <typename T, typename Surject>
void Set<T,Surject>::SetSurject (Surject surject)
{
    this->surject = std::move (surject);

    for (typename ForwardList<SetNode<T>>::Iterator it = nodes.Begin (); it != nodes.End (); ++it)
    {
        it->cachedHash = surject (it->elem);
    }
    Rehash ();
}

template <typename T, typename Surject>
typename Set<T,Surject>::ConstIterator Set<T,Surject>::CBegin () const
{
    Set<T,Surject>* pThis = const_cast<Set<T,Surject>*> (this);
    return ConstIterator (pThis->nodes.Begin ());
}

template <typename T, typename Surject>
typename Set<T,Surject>::ConstIterator Set<T,Surject>::CEnd () const
{
    Set<T,Surject>* pThis = const_cast<Set<T,Surject>*> (this);
    return ConstIterator (pThis->nodes.End ());
}

template <typename T, typename Surject>
float Set<T,Surject>::LoadFactor () const
{
    assert (numBuckets != 0);
    return float (nodes.Size ()) / float (numBuckets);
}

template <typename T, typename Surject>
void Set<T,Surject>::BalanceLoadFactor ()
{
    const float highLoadFactor = 0.7f;

    if (numBuckets==0 || LoadFactor () >= highLoadFactor)
    {
        Rehash ();
    }
}

template <typename T, typename Surject>
size_t Set<T,Surject>::OptimalBucketCnt (size_t elems)
{
    const float optimalLoadFactor = 0.3f;
    const size_t bucketThreshold = 50;

    size_t res = elems * (1.f / optimalLoadFactor);
    return std::max (bucketThreshold, res);
}

template <typename T, typename Surject>
void Set<T,Surject>::Rehash ()
{
    Set tmp (nodes.Size ());
    for (InternalIterator it = nodes.Begin (); it != nodes.End (); ++it)
    {
        tmp.InsertNode (it->cachedHash, std::move (it->elem));
    }
    Swap (tmp);
}

