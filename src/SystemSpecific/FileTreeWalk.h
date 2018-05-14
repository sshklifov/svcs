#ifndef FILE_TREE_WALK_INCLUDED
#define FILE_TREE_WALK_INCLUDED

#include "FilesystemFunctions.h"

#include <sys/types.h>
#include <dirent.h>
#include <string>

class FileTreeWalk
{
public:
    FileTreeWalk ();
    FileTreeWalk (std::string s);

    FileTreeWalk (const FileTreeWalk&) = delete;
    void operator= (const FileTreeWalk&) = delete;

    FileTreeWalk (FileTreeWalk&& rhs);
    FileTreeWalk& operator= (FileTreeWalk&& rhs);

    ~FileTreeWalk ();

    void AssignDir (std::string s);

    void Advance ();
    bool IsFinished () const;
    void Finish ();

    bool IsDirectory () const;
    const char* GetBasename () const;
    const char* GetDirname () const;

private:
    void Stat (struct stat* res) const;
    void Swap (FileTreeWalk& rhs);

private:
    std::string root;
    DIR* dir;
    struct dirent* entry;
};

inline std::string GetFileName (const FileTreeWalk& it)
{
    std::string res;
    res.reserve (GetMaxPathname ());

    return Join (it.GetDirname (), it.GetBasename ());
}

#endif /* FILE_TREE_WALK_INCLUDED */
