#include "GetTime.h"

#include <ctime>
#include <cassert>

std::ostream& GetTime (std::ostream& os)
{
    const unsigned BUF_SIZE = 128;
    char buf[BUF_SIZE];

    time_t now = time (NULL);
    if (now == -1)
    {
        throw std::runtime_error ("GetTime: could not tell time");
    }

    struct tm * timeStruct = localtime (&now);
    assert (strftime (buf, BUF_SIZE, "%R(%Z) %d.%m.%Y", timeStruct) != 0);

    os << buf;
    return os;
}

