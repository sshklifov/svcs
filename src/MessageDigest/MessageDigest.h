#ifndef MESSAGE_DIGEST_INCLUDED
#define MESSAGE_DIGEST_INCLUDED

#include <vector>
#include <string>

extern std::vector<unsigned char> DigestFile (const char* file);
extern std::string HexEncodeDigest (const std::vector<unsigned char>& digest);
extern std::vector<unsigned char> HexDecodeDigest (const char* buf);
extern bool CompareDigests (const std::vector<unsigned char>& x, const std::vector<unsigned char>& y);

class WriteDigest
{
    friend std::ostream& operator<< (std::ostream& os, WriteDigest&& manip);

public:
    WriteDigest (const std::vector<unsigned char>& digest);

    WriteDigest () = delete;
    WriteDigest (const WriteDigest&) = delete;
    void operator= (const WriteDigest&) = delete;

private:
    const std::vector<unsigned char>& digest;
};

std::ostream& operator<< (std::ostream& os, WriteDigest&& manip);

class ReadDigest
{
    friend std::istream& operator >> (std::istream& is, ReadDigest&& manip);

public:
    ReadDigest (std::vector<unsigned char>& digest);

    ReadDigest () = delete;
    ReadDigest (const ReadDigest&) = delete;
    void operator= (const ReadDigest&) = delete;

private:
    std::vector<unsigned char>& digest;
};

std::istream& operator>> (std::istream& is, ReadDigest&& manip);

#endif /* MESSAGE_DIGEST_INCLUDED */
