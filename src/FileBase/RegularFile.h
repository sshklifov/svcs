#ifndef REGULAR_FILE_INCLUDED
#define REGULAR_FILE_INCLUDED

#include "FileBase.h"

#include <vector>
#include <string>

class RegularFile : public FileBase
{
public:
    RegularFile (std::string s, std::vector<unsigned char> digest);

    RegularFile (const RegularFile& rhs) = default;
    RegularFile (RegularFile&& rhs) = default;
    RegularFile& operator= (const RegularFile& rhs) = default;
    RegularFile& operator= (RegularFile&& rhs) = default;

    virtual ~RegularFile () = default;

    virtual const char* GetName () const;
    virtual bool Identify (const char* s) const;
    virtual const char* GetUID () const;
    virtual FileBase* Clone () const;

    const std::vector<unsigned char>& GetDigest () const;

private:
    std::string name;
    std::vector<unsigned char> digest;

public:
    static constexpr char stringId[] = "regular_file";
};

#endif /* REGULAR_FILE_INCLUDED */
