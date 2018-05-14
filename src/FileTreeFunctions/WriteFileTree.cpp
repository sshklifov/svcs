#include "WriteFileTree.h"

#include <MessageDigest.h>
#include <Manipulators.h>

static void WriteFileTreeRec (const FileBase* root, std::ostream& os)
{
    assert (os.exceptions () == (std::ios_base::failbit|std::ios_base::badbit|std::ios_base::eofbit));

    if (root->Identify (RegularFile::stringId))
    {
        const RegularFile* regularFile = dynamic_cast<const RegularFile*> (root);
        assert (regularFile != NULL);

        os << regularFile->GetName () << ':';
        os << WriteDigest (regularFile->GetDigest ());
        return;
    }

    const Directory* dirFile = dynamic_cast<const Directory*> (root);
    assert (dirFile != NULL);

    os << PrintFmt ("(%s){", dirFile->GetName ());
    for (Directory::ConstIterator it = dirFile->CBegin (); it != dirFile->CEnd (); ++it)
    {
        WriteFileTree (*it, os);
        if (std::next (it) != dirFile->CEnd ())
        {
            os << ',';
        }
    }
    os << '}';
}

void WriteFileTree (const FileBase* root, std::ostream& os)
{
    std::ios_base::iostate state = os.exceptions ();
    os.exceptions (std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

    WriteFileTreeRec (root, os);
    os.exceptions (state);
}

WriteFileTreeManip::WriteFileTreeManip (const FileBase* root) : root (root)
{
}

std::ostream& operator << (std::ostream& os, const WriteFileTreeManip& manip)
{
    WriteFileTree (manip.root, os);
    return os;
}
