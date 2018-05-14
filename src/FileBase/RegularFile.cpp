#include "RegularFile.h"

#include <cstring>

constexpr char RegularFile::stringId[];

RegularFile::RegularFile (std::string s, std::vector<unsigned char> digest) :
    name (std::move (s)),
    digest (std::move (digest))
{
}

const char* RegularFile::GetName () const
{
    return name.c_str ();
}

bool RegularFile::Identify (const char* s) const
{
    return strcmp (s, stringId) == 0;
}

const char* RegularFile::GetUID () const
{
    return stringId;
}

FileBase* RegularFile::Clone () const
{
    return new RegularFile (*this);
}

const std::vector<unsigned char>& RegularFile::GetDigest () const
{
    return digest;
}
