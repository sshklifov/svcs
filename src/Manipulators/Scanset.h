#ifndef SCANSET_INCLUDED
#define SCANSET_INCLUDED

#include <BracketExpr.h>

#include <istream>
#include <string>

class Scanset
{
    friend std::istream& operator>> (std::istream& is, Scanset&& rhs);

public:
    enum class Flags {OVERWRITE, APPEND};

public:
    Scanset (const char* fmt, std::nullptr_t);
    Scanset (const char* fmt, std::string* pStr, Flags flag = Flags::OVERWRITE);

    Scanset () = delete;
    Scanset (const Scanset&) = delete;
    void operator= (const Scanset&) = delete;

private:
    void Append (char c);
    void Ignore (char c);
    void DecideWhatToDo (char c);

private:
    BracketExpr charSet;
    std::string* pStr;
    void (Scanset::*DoThis) (char);
};

std::istream& operator>> (std::istream& is, Scanset&& manip);

#endif /* SCANSET_INCLUDED */
