#ifndef SET_NODE_INCLUDED
#define SET_NODE_INCLUDED

#include <cstddef>
#include <utility>

template <typename T>
struct SetNode
{
public:
    SetNode () = delete;
    SetNode (const SetNode&) = delete;
    void operator= (const SetNode&) = delete;

    SetNode (SetNode&&) = default;
    SetNode& operator= (SetNode&&) = default;

    SetNode (size_t cachedHash, const T& elem) : cachedHash (cachedHash), elem (elem) {}
    SetNode (size_t cachedHash, T&& elem) : cachedHash (cachedHash), elem (std::move (elem)) {}

public:
    size_t cachedHash;
    T elem;
};

#endif /* SET_NODE_INCLUDED */
