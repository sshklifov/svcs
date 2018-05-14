#include "Directory.h"

#include <cstring>

constexpr char Directory::stringId[];

Directory::Directory (std::string s) : name (std::move (s))
{
}

const char* Directory::GetName () const
{
    return name.c_str ();
}

bool Directory::Identify (const char* s) const
{
    return strcmp (s, stringId) == 0;
}

const char* Directory::GetUID () const
{
    return stringId;
}

FileBase* Directory::Clone () const
{
    return new Directory (*this);
}
