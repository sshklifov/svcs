#include "FileHashTableNode.h"
#include <FileTypes.h>

#include <HashFunctions.h>
#include <RegularFile.h>
#include <MessageDigest.h>

#include <stdexcept>
#include <cstring>
#include <cassert>

FileHashTableNode::FileHashTableNode (FileBase* pFile) : pFile (pFile)
{
}

FileHashTableNode::FileHashTableNode (const FileHashTableNode& rhs) : pFile (rhs.pFile->Clone ())
{
}
FileHashTableNode::FileHashTableNode (FileHashTableNode&& rhs)
{
    pFile = rhs.pFile;
    rhs.pFile = NULL;
}

FileHashTableNode& FileHashTableNode::operator= (const FileHashTableNode& rhs)
{
    if (this != &rhs)
    {
        delete pFile;
        pFile = rhs.pFile->Clone ();
    }

    return (*this);
}

FileHashTableNode& FileHashTableNode::operator= (FileHashTableNode&& rhs)
{
    if (this != &rhs)
    {
        delete pFile;
        pFile = rhs.pFile;
        rhs.pFile = NULL;
    }

    return (*this);
}

FileHashTableNode::~FileHashTableNode ()
{
    delete pFile;
}

bool FileHashTableNode::operator== (const FileHashTableNode& rhs) const
{
    if (strcmp (pFile->GetUID (), rhs.pFile->GetUID ()) == 0)
    {
        if (strcmp (static_cast<const FileBase*>(pFile)->GetName (), static_cast<const FileBase*>(rhs.pFile)->GetName ()) != 0)
        {
            return false;
        }

        bool isRegular = pFile->Identify (RegularFile::stringId);
        if (isRegular)
        {
            return CompareDigests (CastToRegular()->GetDigest (), rhs.CastToRegular()->GetDigest ());
        }
        else return true;
    }
    else return false;
}

bool FileHashTableNode::operator!= (const FileHashTableNode& rhs) const
{
    return !((*this)==rhs);
}

FileHashTableNode::operator const FileBase* () const
{
    return pFile;
}

FileHashTableNode::operator FileBase* ()
{
    return pFile;
}

const FileBase* FileHashTableNode::operator-> () const
{
    return pFile;
}

FileBase* FileHashTableNode::operator-> ()
{
    return pFile;
}

const RegularFile* FileHashTableNode::CastToRegular () const
{
    const RegularFile* res = dynamic_cast<const RegularFile*> (pFile);
    assert (res != NULL);
    return res;
}

RegularFile* FileHashTableNode::CastToRegular ()
{
    const FileHashTableNode* constThis = const_cast<const FileHashTableNode*> (this);
    return const_cast<RegularFile*> (constThis->CastToRegular ());
}

const Directory* FileHashTableNode::CastToDirectory () const
{
    const Directory* res = dynamic_cast<const Directory*> (pFile);
    assert (res != NULL);
    return res;
}

Directory* FileHashTableNode::CastToDirectory ()
{
    const FileHashTableNode* constThis = const_cast<const FileHashTableNode*> (this);
    return const_cast<Directory*> (constThis->CastToDirectory ());
}

FileBase* FileHashTableNode::Release ()
{
    FileBase* res = pFile;
    pFile = NULL;
    return res;
}

void FileHashTableNode::Reset (FileBase* p)
{
    delete pFile;
    pFile = p;
}
