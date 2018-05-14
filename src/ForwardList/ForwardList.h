#ifndef FORWARD_LIST_INCLUDED
#define FORWARD_LIST_INCLUDED

#include <initializer_list>

#include "ForwardListNode.h"

template <typename T>
class ForwardListIterator;
#include "ForwardListIterator.h"

template <typename T>
class ForwardListConstIterator;
#include "ForwardListConstIterator.h"

template <typename T>
class ForwardList
{
public:
    using Iterator = ForwardListIterator<T>;
    using ConstIterator = ForwardListConstIterator<T>;

public:
    ForwardList ();
    ForwardList (std::initializer_list<T> init);

    template <typename ForwardIt>
    ForwardList (ForwardIt first, ForwardIt last);

    ForwardList (const ForwardList& rhs);
    ForwardList (ForwardList&& rhs);
    ForwardList& operator= (const ForwardList& rhs);
    ForwardList& operator= (ForwardList&& rhs);

    ~ForwardList ();

    Iterator BeforeBegin ();
    Iterator Begin ();
    Iterator End ();
    ConstIterator CBeforeBegin () const;
    ConstIterator CBegin () const;
    ConstIterator CEnd () const;

    T& Front ();
    const T& Front () const;
    bool Empty () const;
    size_t Size () const;

    template <typename... Args>
    void InsertAfter (ConstIterator it, Args&&... args);

    template <typename... Args>
    void InsertFront (Args&&... args);

    template <typename... Args>
    void InsertSentinel (Args&&... args);

    void RemoveAfter (ConstIterator it);
    void RemoveFront ();

    template <typename ForwardIt>
    void Assign (ForwardIt first, ForwardIt last);

    void Clear ();
    void Swap (ForwardList& rhs);

private:
    ForwardListNode<T>* sentinel;
    size_t size;
};

#include "ForwardList.inl"

#endif /* FORWARD_LIST_INCLUDED */
