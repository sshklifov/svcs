#include "PrintFmt.h"

std::ostream& operator<< (std::ostream& os, const PrintFmt& manip)
{
    const char* s = manip.s.get ();
    if (s != NULL)
    {
        os << s;
    }
    else
    {
        os.setstate (manip.state);
    }

    return os;
}

