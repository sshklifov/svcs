#include "HashPointer.h"

size_t HashPointer (const void* p)
{
    constexpr size_t maxAlignment = alignof (std::max_align_t);
    return reinterpret_cast<size_t> (p) >> maxAlignment;
}
