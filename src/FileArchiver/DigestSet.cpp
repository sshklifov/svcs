#include "DigestSet.h"
#include <HashDigest.h>

size_t DigestSetSurject::operator() (const std::vector<unsigned char>& digest) const
{
    return HashDigest (digest);
}

