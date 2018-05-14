#include "FilesystemFunctions.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <cerrno>
#include <cstring>

#include <stdexcept>
#include <iostream>

static std::ostream& LogErrno (std::ostream& ostr)
{
    ostr << "errno=" << strerror (errno) << "\n";
    return ostr;
}

bool FileExists (const char* name)
{
    struct stat st;
    int retval = stat (name, &st);
    if (retval != -1)
    {
        return true;
    }
    else if (errno == ENOENT)
    {
        errno = 0;
        return false;
    }
    else
    {
        std::cerr << "FileExists: failed with name=\'" << name << "\'\n" << LogErrno;
        throw std::runtime_error ("FileExists: stat failed");
    }
}

bool DirectoryExists (const char* name)
{
    return FileExists (name);
}

void CreateFile (const char* name)
{
    int retval = creat (name, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (retval == -1)
    {
        std::cerr << "CreateFile: failed with name=\'" << name << "\'\n" << LogErrno;
        throw std::runtime_error ("CreateFile: creat failed");
    }
}

void CreateDirectory (const char* name)
{
    int retval = mkdir (name, S_IRWXU | S_IRWXG | S_IRWXO);
    if (retval == -1)
    {
        std::cerr << "CreateDirectory: failed with name=\'" << name << "\'\n" << LogErrno;
        throw std::runtime_error ("CreateDirectory: mkdir failed");
    }
}

long GetMaxPathname ()
{
    class MaxPathnameClass
    {
    public:
        MaxPathnameClass ()
        {
            val = pathconf ("/", _PC_PATH_MAX);
            if (val == -1)
            {
                throw std::runtime_error ("GetMaxPathname: pathconf failed");
            }
        }

        long Get () const
        {
            return val;
        }

    private:
        long val;

    } static maxPathname;

    return maxPathname.Get ();
}

void RemoveFile (const char* name)
{
    int retval = unlink (name);
    if (retval == -1)
    {
        std::cerr << "RemoveFile: failed with name=\'" << name << "\'\n" << LogErrno;
        throw std::runtime_error ("RemoveFile: unlink failed");
    }
}

void RemoveDirectory (const char* name)
{
    int retval = rmdir (name);
    if (retval == -1)
    {
        std::cerr << "RemoveDirectory: failed with name=\'" << name << "\'\n" << LogErrno;
        throw std::runtime_error ("RemoveDirectory: rmdir failed");
    }
}
