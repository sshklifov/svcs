#ifndef EXACT_MATCH_INCLUDED
#define EXACT_MATCH_INCLUDED

#include <istream>
#include <cstring>
#include <cassert>

class ExactMatch
{
    friend std::istream& operator>> (std::istream& is, const ExactMatch& manip)
    {
        char buf[manip.size];
        is.read (buf, manip.size);

        if (is && memcmp (buf, manip.s, manip.size) != 0)
        {
            is.setstate (std::ios_base::failbit);
        }

        return is;
    }

public:
    template <size_t N>
    ExactMatch (const char (&literal)[N])
    {
        assert (strlen (literal) + 1 == N);
        s = literal;
        size = N - 1;
    }

    ExactMatch (const char* s, unsigned size) : s (s), size (size) {}
    ExactMatch (const ExactMatch&) = default;

    ExactMatch () = delete;
    void operator= (const ExactMatch&) = delete;

private:
    const char* s;
    unsigned size;
};

#endif /* EXACT_MATCH_INCLUDED */
