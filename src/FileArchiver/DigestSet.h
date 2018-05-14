#ifndef DIGEST_SET_INCLUDED
#define DIGEST_SET_INCLUDED

#include <Set.h>
#include <vector>

struct DigestSetSurject
{
    size_t operator() (const std::vector<unsigned char>& digest) const;
};

using DigestSet = Set<std::vector<unsigned char>,DigestSetSurject>;

#endif /* DIGEST_SET_INCLUDED */
