#include "ArchiveFileTreeHelper.h"
#include "DigestSet.h"

#include <WorkingDir.h>
#include <FileTreeWalk.h>
#include <MessageDigest.h>

class ArchiveFileTreeHelperClass
{
public:
    ArchiveFileTreeHelperClass (std::string& dirname, DigestSet& digests, ArchiveFileCb cb);
    void operator() (const Directory* tree);

private:
    std::string& dirname;
    DigestSet& digests;
    ArchiveFileCb cb;
};

ArchiveFileTreeHelperClass::ArchiveFileTreeHelperClass (std::string& dirname, DigestSet& digests, ArchiveFileCb cb) :
    dirname (dirname),
    digests (digests),
    cb (cb)
{
}

void ArchiveFileTreeHelperClass::operator() (const Directory* tree)
{
    for (Directory::ConstIterator it = tree->CBegin (); it != tree->CEnd (); ++it)
    {
        if ((*it)->Identify (RegularFile::stringId))
        {
            if (digests.Find (it->CastToRegular()->GetDigest ()) == digests.CEnd ())
            {
                size_t oldSize = Join (dirname, (*it)->GetName ());
                cb (dirname.c_str (), it->CastToRegular()->GetDigest ());
                dirname.resize (oldSize);

                digests.Insert (it->CastToRegular()->GetDigest ());
            }
        }
        else
        {
            size_t oldSize = Join (dirname, (*it)->GetName ());
            (*this)(it->CastToDirectory ());
            dirname.resize (oldSize);
        }
    }
}

void ArchiveFileTreeHelper (const Directory* tree, ArchiveFileCb cb)
{
    DigestSet digests;
    FileTreeWalk it (WorkingDir::GetInstance().GetFileArchiverDir ());
    while (!it.IsFinished ())
    {
        assert (!it.IsDirectory ());

        digests.Insert (HexDecodeDigest (it.GetBasename ()));
        it.Advance ();
    }

    std::string dirname = WorkingDir::GetInstance().GetWorkingDir ();
    ArchiveFileTreeHelperClass(dirname, digests, cb)(tree);
}
