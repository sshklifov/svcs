#include "FileTreeWalk.h"

#include <sys/stat.h>

#include <cstring>
#include <cassert>
#include <cerrno>

#include <stdexcept>
#include <iostream>

FileTreeWalk::FileTreeWalk () : dir (NULL), entry (NULL)
{
}

FileTreeWalk::FileTreeWalk (std::string s) : FileTreeWalk ()
{
    AssignDir (std::move (s));
}

void FileTreeWalk::AssignDir (std::string s)
{
    Finish ();

    root = std::move (s);
    dir = opendir (root.c_str ());
    if (dir == NULL)
    {
        std::cerr << "FileTreeWalk::AssignDir: could not open directory " << root.c_str () << "\n";
        throw std::runtime_error ("FileTreeWalk::AssignDir failed");
    }

    Advance ();
}

FileTreeWalk::FileTreeWalk (FileTreeWalk&& rhs) : FileTreeWalk ()
{
    Swap (rhs);
}
FileTreeWalk& FileTreeWalk::operator= (FileTreeWalk&& rhs)
{
    Swap (rhs);
    return (*this);
}

void FileTreeWalk::Swap (FileTreeWalk& rhs)
{
    root.swap (rhs.root);

    std::swap (dir, rhs.dir);
    std::swap (entry, rhs.entry);
}

FileTreeWalk::~FileTreeWalk ()
{
    Finish ();
}

void FileTreeWalk::Advance ()
{
    int savedErrno = errno;
    assert (!IsFinished ());

    do
    {
        errno = 0;
        entry = readdir (dir);
        assert (errno == 0);
    }
    while (entry && entry->d_name[0] == '.');

    if (!entry) Finish ();
    errno = savedErrno;
}

void FileTreeWalk::Finish ()
{
    if (!IsFinished ())
    {
        int retval = closedir (dir);
        assert (retval != -1);
        dir = NULL;
        entry = NULL;
    }
}

bool FileTreeWalk::IsFinished () const
{
    return dir == NULL;
}

bool FileTreeWalk::IsDirectory () const
{
    assert (!IsFinished ());

#ifdef _DIRENT_HAVE_D_TYPE
    return entry->d_type == DT_DIR;
#else
    struct stat st;
    Stat (&st);
    return S_ISDIR (st.st_mode);
#endif
}

void FileTreeWalk::Stat (struct stat* res) const
{
    assert (!IsFinished ());

    std::string name = GetFileName (*this);

    int retval;
    retval = stat (name.c_str (), res);
    if (retval == -1)
    {
        std::cerr << "FileTreeWalk::Stat: could not stat file " << name.c_str () << "\n";
        throw std::runtime_error ("FileTreeWalk: stat failed");
    }
}

const char* FileTreeWalk::GetBasename () const
{
    assert (!IsFinished ());
    return entry->d_name;
}

const char* FileTreeWalk::GetDirname () const
{
    return root.c_str ();
}
