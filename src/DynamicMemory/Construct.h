#ifndef CONSTRUCT_INCLUDED
#define CONSTRUCT_INCLUDED

#include <cstddef>
#include <utility>

template <typename T, typename... Args>
void Construct (T* p, Args&&... args)
{
    p = new (p) T (std::forward<Args> (args)...);
}

template <typename T>
void Destroy (T* p, size_t n = 1)
{
    while (n)
    {
        p->~T ();

        ++p;
        --n;
    }
}

#endif /* CONSTRUCT_INCLUDED */
