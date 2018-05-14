#include "HashDigest.h"

#include <cmath>
#include <algorithm> // std::min

size_t HashDigest (const std::vector<unsigned char>& digest)
{
    const size_t numBytes = sizeof (size_t);

    size_t res = 0;
    size_t shift = 8;

    size_t cycleTo = std::min (digest.size (), numBytes);
    for (size_t i = 0; i < cycleTo; ++i)
    {
        res <<= shift;
        res |= digest[i];
    }

    return res;
}
