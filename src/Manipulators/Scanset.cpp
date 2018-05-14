#include "Scanset.h"
#include <cassert>

Scanset::Scanset (const char* fmt, std::nullptr_t) : pStr (NULL)
{
    DoThis = &Scanset::Ignore;
    BuildBracketExpr (charSet, fmt);
}

Scanset::Scanset (const char* fmt, std::string* pStr, Flags flag) : pStr (pStr)
{
    if (pStr)
    {
        DoThis = &Scanset::Append;
        switch (flag)
        {
        case Flags::OVERWRITE:
            pStr->clear ();
            break;

        case Flags::APPEND:
            break;

        default:
            assert ("invalid flag");
        }
    }
    else
    {
        DoThis = &Scanset::Ignore;
    }

    BuildBracketExpr (charSet, fmt);
}

void Scanset::Append (char c)
{
    pStr->push_back (c);
}

void Scanset::Ignore (char)
{
}

void Scanset::DecideWhatToDo (char c)
{
    (*this.*DoThis) (c);
}

std::istream& operator>> (std::istream& is, Scanset&& manip)
{
    char c;
    while ((c=is.peek ()) != EOF && BracketExprContainsChar (manip.charSet, c))
    {
        manip.DecideWhatToDo (c);
        is.ignore ();
    }

    return is;
}
