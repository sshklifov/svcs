#include "PrintFmt.h"

#include <memory>
#include <cassert>
#include <cstring>

template <typename... Args>
PrintFmt::PrintFmt (const char* fmt, Args... args)
{
    try
    {
        const size_t DEFAULT_SIZE = 1024;
        std::unique_ptr<char> buf (new char[DEFAULT_SIZE]);
        size_t size = DEFAULT_SIZE;

        int retval;

        retval = snprintf (buf.get (), size, fmt, args...);
        if (retval < 0)
        {
            state |= std::ios_base::badbit;
            return;
        }

        if (size_t (retval) >= size)
        {
            size = retval + 1;
            buf.reset (new char[size]);

            retval = snprintf (buf.get (), size, fmt, args...);
            if (retval < 0)
            {
                state |= std::ios_base::badbit;
                return;
            }
            assert (size_t (retval) < size);
        }

        s.reset (buf.release ());
    }
    catch (std::bad_alloc& e)
    {
        state |= std::ios_base::failbit;
        throw;
    }
}
