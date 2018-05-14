#include "Ignore.h"
#include <limits>


Ignore::Ignore (char c, unsigned times) : c (c), times (times)
{
}

std::istream& operator>> (std::istream& is, Ignore&& manip)
{
    while (manip.times && is)
    {
        is.ignore (std::numeric_limits<std::streamsize>::max (), manip.c);
        --manip.times;
    }

    return is;
}
