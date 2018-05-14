#ifndef PRINT_FMT_INCLUDED
#define PRINT_FMT_INCLUDED

#include <ostream>
#include <string>
#include <memory>

class PrintFmt
{
    friend std::ostream& operator<< (std::ostream& os, const PrintFmt& manip);

public:
    template <typename... Args>
    PrintFmt (const char* fmt, Args... args);

    PrintFmt () = delete;
    PrintFmt (const PrintFmt&) = delete;
    void operator= (const PrintFmt&) = delete;

private:
    std::unique_ptr<char> s;
    std::ios_base::iostate state = std::ios_base::goodbit;
};

std::ostream& operator<< (std::ostream& os, const PrintFmt& manip);

#include "PrintFmt.inl"

#endif /* PRINT_FMT_INCLUDED */
