#include "HashString.h"

#include <cstdint>

size_t HashString (const char* s)
{
    const uint32_t BASE = 131;
    const uint32_t MOD = 32785993;

    uint32_t res = 0;
    for (size_t i = 0; s[i]; ++i)
    {
        res *= BASE;
        res += s[i];
        res %= MOD;
    }

    return res;
}
