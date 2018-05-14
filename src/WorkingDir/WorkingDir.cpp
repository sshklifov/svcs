#include "WorkingDir.h"

#include <CommitLog.h>
#include <FileArchiver.h>

#include <FilesystemFunctions.h>
#include <stdexcept>
#include <cassert>

using Singleton = WorkingDir::Singleton;

Singleton& WorkingDir::GetInstance ()
{
    static Singleton instance;
    return instance;
}

void Singleton::FirstTimeInitialize (const char* dir)
{
    std::string s = dir;
    s += "/.svcs";

    if (DirectoryExists (s.c_str ()))
    {
        throw std::runtime_error ("WorkingDir::FirstTimeInitialize: the specified directory has already been initialized");
    }
    CreateDirectory (s.c_str ());

    s.resize (s.find_last_of ('/'));
    InitializeStrings (std::move (s));

    CreateFile (commitLogCsv.c_str ());
    CreateFile (commitLogMetadata.c_str ());
    CommitLog::GetInstance().FirstTimeInitialize ();

    CreateDirectory (fileArchiverDir.c_str ());
    FileArchiver::GetInstance().FirstTimeInitialize ();
}

void Singleton::Initialize (const char* dir)
{
    std::string s = dir;
    s += "/.svcs";

    if (!DirectoryExists (s.c_str ()))
    {
        throw std::runtime_error ("WorkingDir::Initialize: invalid directory, did you init?");
    }

    s.resize (s.find_last_of ('/'));
    InitializeStrings (std::move (s));
}

void Singleton::InitializeStrings (std::string dir)
{
    workingDir = std::move (dir);
    commitLogCsv = workingDir + "/.svcs/commit_log.csv";
    commitLogMetadata = workingDir + "/.svcs/commit_log.metadata";
    fileArchiverDir = workingDir + "/.svcs/archive";
}

const char* Singleton::GetWorkingDir ()
{
    assert (!workingDir.empty ());
    return workingDir.c_str ();
}

const char* Singleton::GetCommitLogCsv ()
{
    assert (!commitLogCsv.empty ());
    return commitLogCsv.c_str ();
}

const char* Singleton::GetCommitLogMetadata ()
{
    assert (!commitLogMetadata.empty ());
    return commitLogMetadata.c_str ();
}

const char* Singleton::GetFileArchiverDir ()
{
    assert (!fileArchiverDir.empty ());
    return fileArchiverDir.c_str ();
}
