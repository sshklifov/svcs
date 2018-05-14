#include "ReadFileTree.h"

#include <Manipulators.h>
#include <MessageDigest.h>

#include <string>
#include <memory>
#include <cassert>

static Directory* ReadFileTreeRec (std::istream& is)
{
    assert (is.exceptions () == (std::ios_base::failbit|std::ios_base::badbit|std::ios_base::eofbit));

    std::string s;
    is >> ExactMatch ("(") >> Scanset ("[^)]", &s) >> ExactMatch ("){");
    std::unique_ptr<Directory> root (new Directory (std::move (s)));

    char delim = is.peek ();
    while (delim != '}')
    {
        if (is.peek () == '(')
        {
            root->Insert (ReadFileTreeRec (is));
            is >> delim;
        }
        else
        {
            is >> Scanset ("[^:]", &s) >> delim;
            std::vector <unsigned char> digest;
            is >> ReadDigest (digest) >> delim;

            root->Insert (new RegularFile (std::move (s), std::move (digest)));
        }
    }

    return root.release ();
}

Directory* ReadFileTree (std::istream& is)
{
    std::ios_base::iostate state = is.exceptions ();
    is.exceptions (std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

    std::unique_ptr<Directory> res (ReadFileTreeRec (is));
    is.exceptions (state);

    return res.release ();
}

ReadFileTreeManip::ReadFileTreeManip (Directory*& res) : res (res)
{
}

std::istream& operator>> (std::istream& is, ReadFileTreeManip&& manip)
{
    manip.res = ReadFileTree (is);
    return is;
}
