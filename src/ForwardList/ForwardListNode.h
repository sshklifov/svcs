#ifndef FORWARD_LIST_NODE_INCLUDED
#define FORWARD_LIST_NODE_INCLUDED

template <typename T>
struct ForwardListNode
{
    ForwardListNode* link;
    T val;
};

#endif /* FORWARD_LIST_NODE_INCLUDED */
