#include "MessageDigest.h"

#include <openssl/evp.h>
#include <openssl/err.h>

#include <Manipulators.h>

#include <sstream>
#include <iostream>
#include <fstream>

#include <memory>
#include <cstring>

static void HandleLibcryptoError ()
{
    unsigned err = ERR_peek_error ();
    char buf[256];
    ERR_error_string_n (err, buf, 256);

    std::cerr << "Libcrypto error: " << buf << "\n";
    throw std::runtime_error ("libcrypto internal error");
}

static std::vector<unsigned char> DigestFileHelper (EVP_MD_CTX* mdctx, const char* file)
{
    std::vector<unsigned char> digest;
    digest.resize (EVP_MD_size (EVP_sha256 ()));

    std::ifstream ifile (file, std::ios_base::in);
    if (!ifile)
    {
        std::cerr << "DigestFile: could not open " << file << "\n";
        throw std::runtime_error ("DigestFile: failed");
    }

    const size_t BUF_SIZE = 4096;
    char buf[BUF_SIZE];
    while (ifile)
    {
        ifile.read (buf, BUF_SIZE-1);
        if (ifile || ifile.eof ())
        {
            if (EVP_DigestUpdate (mdctx, buf, ifile.gcount ()) != 1)
                HandleLibcryptoError ();
        }
    }
    if (!ifile.eof ())
    {
        throw std::runtime_error ("DigestFileHelper: i/o error");
    }

    unsigned digestSize;
    if (EVP_DigestFinal_ex (mdctx, digest.data (), &digestSize) != 1)
        HandleLibcryptoError ();

    digest.resize (digestSize);
    digest.shrink_to_fit ();
    return digest;
}

std::vector<unsigned char> DigestFile (const char* file)
{
    struct Deleter
    {
        void operator() (EVP_MD_CTX* p)
        {
            EVP_MD_CTX_free (p);
        }
    };

    std::unique_ptr<EVP_MD_CTX, Deleter> mdctx (EVP_MD_CTX_new ());
    if (mdctx == NULL)
        HandleLibcryptoError ();

    if (EVP_DigestInit_ex (mdctx.get (), EVP_sha256 (), NULL) != 1)
        HandleLibcryptoError ();

    return DigestFileHelper (mdctx.get (), file);
}

std::string HexEncodeDigest (const std::vector<unsigned char>& digest)
{
    std::ostringstream oss;
    oss << WriteDigest (digest);
    return oss.str ();
}

std::vector<unsigned char> HexDecodeDigest (const char* buf)
{
    std::vector<unsigned char> res;
    for (unsigned i = 0; buf[i];)
    {
        int numRead;
        unsigned char hexadec;
        if (sscanf (buf + i, "%2hhx%n", &hexadec, &numRead) != 1)
        {
            throw std::runtime_error ("HexDecodeDigest: sscanf failed");
        }

        res.push_back (hexadec);
        i += numRead;
    }

    res.shrink_to_fit ();
    return res;
}

bool CompareDigests (const std::vector<unsigned char>& x, const std::vector<unsigned char>& y)
{
    if (x.size () != y.size ()) return false;
    return memcmp (x.data (), y.data (), sizeof (unsigned char) * x.size ()) == 0;
}

WriteDigest::WriteDigest (const std::vector<unsigned char>& digest) : digest (digest)
{
}

std::ostream& operator<< (std::ostream& os, WriteDigest&& manip)
{
    for (size_t i = 0; i < manip.digest.size () && os; ++i)
    {
        os << PrintFmt ("%.2x", manip.digest[i]);
    }

    return os;
}

ReadDigest::ReadDigest (std::vector<unsigned char>& digest) : digest (digest)
{
    digest.clear ();
}


std::istream& operator>> (std::istream& is, ReadDigest&& manip)
{
    std::string s;
    is >> Scanset ("[0-9a-eA-E]", &s);

    if (is)
    {
        manip.digest = HexDecodeDigest (s.c_str ());
    }
    return is;
}
