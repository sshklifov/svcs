#ifndef TEMPLATED_MALLOC_INCLUDED
#define TEMPLATED_MALLOC_INCLUDED

#include <cstdlib>
#include <cassert>
#include <new>

template <typename T>
T* TemplatedMalloc (size_t cnt = 1)
{
    assert (cnt != 0);

    T* res = (T*)malloc (sizeof (T) * cnt);
    if (!res) throw std::bad_alloc ();
    return res;
}

#endif /* TEMPLATED_MALLOC_INCLUDED */
