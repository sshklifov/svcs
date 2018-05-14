#ifndef SET_INCLUDED
#define SET_INCLUDED

#include <ForwardList.h>

#include "SetNode.h"

template <typename T>
class SetConstIterator;
#include "SetConstIterator.h"

template <typename T, typename Surject>
class Set
{
public:
    using ConstIterator = SetConstIterator<T>;

public:
    Set ();
    Set (Surject surject);
    Set (size_t hint);
    ~Set ();

    Set (const Set& rhs);
    Set& operator= (const Set& rhs);
    Set (Set&& rhs);
    Set& operator= (Set&& rhs);

    template <typename... Args>
    void Insert (Args&&... args);

    template <typename... Args>
    void Join (Args&&... args);

    void Remove (const T& elem);
    void Remove (ConstIterator it);
    T Release (ConstIterator it);
    void Subtract (const T& elem);

    ConstIterator Find (const T& elem) const;

    template <typename Callable>
    void ForEachInvImg (size_t image, Callable f) const;

    bool Empty () const;
    size_t Size () const;
    void Clear (); 
    void Swap (Set& rhs);

    const Surject& GetSurject () const;
    void SetSurject (Surject surject);

    ConstIterator CBegin () const;
    ConstIterator CEnd () const;

    void Rehash ();
    float LoadFactor () const;

private:
    template <typename... Args>
    void InsertNode (size_t cachedHash, Args&&... args);

    void RemoveAfter (ConstIterator it);
    ConstIterator FindBefore (const T& elem) const;
    void CopyAssign (const Set& rhs);

    static size_t OptimalBucketCnt (size_t elems);
    void BalanceLoadFactor ();
   
private:
    using InternalIterator = typename ForwardList<SetNode<T>>::Iterator;
    using InternalConstIterator = typename ForwardList<SetNode<T>>::ConstIterator;

private:
    ForwardList<SetNode<T>> nodes;
    InternalIterator* buckets;
    size_t numBuckets;
    Surject surject;
};

#include "Set.inl"

#endif /* SET_INCLUDED */
