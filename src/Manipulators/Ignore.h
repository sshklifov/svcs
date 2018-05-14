#ifndef IGNORE_INCLUDED
#define IGNORE_INCLUDED

#include <istream>

class Ignore
{
    friend std::istream& operator>> (std::istream& is, Ignore&& manip);

public:
    Ignore (char c, unsigned times = 1);

    Ignore () = delete;
    Ignore (const Ignore&) = delete;
    void operator= (const Ignore&) = delete;

private:
    char c;
    unsigned times;
};

std::istream& operator>> (std::istream& is, Ignore&& manip);

#endif /* IGNORE_INCLUDED */
