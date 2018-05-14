#ifndef FILE_HASH_TABLE_NODE_INCLUDED
#define FILE_HASH_TABLE_NODE_INCLUDED

#include <cstddef>

class FileBase;
class RegularFile;
class Directory;

class FileHashTableNode
{
public:
    FileHashTableNode (FileBase* pFile = NULL);

    FileHashTableNode (const FileHashTableNode&);
    FileHashTableNode (FileHashTableNode&&);
    FileHashTableNode& operator= (const FileHashTableNode&);
    FileHashTableNode& operator= (FileHashTableNode&&);

    ~FileHashTableNode ();

    bool operator== (const FileHashTableNode& rhs) const;
    bool operator!= (const FileHashTableNode& rhs) const;

    const FileBase* operator-> () const;
    FileBase* operator-> ();
    operator const FileBase* () const;
    operator FileBase* ();

    RegularFile* CastToRegular ();
    const RegularFile* CastToRegular () const;

    Directory* CastToDirectory ();
    const Directory* CastToDirectory () const;

    FileBase* Release ();
    void Reset (FileBase* p);

private:
    FileBase* pFile;
};

#endif /* FILE_HASH_TABLE_NODE_INCLUDED */
