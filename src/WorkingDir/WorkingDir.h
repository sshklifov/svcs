#ifndef WORKING_DIR_INCLUDED
#define WORKING_DIR_INCLUDED

#include <string>

class WorkingDir
{
public:
    class Singleton;
    static Singleton& GetInstance ();
};

class WorkingDir::Singleton
{
public:
    Singleton () = default;
    Singleton (const Singleton&) = delete;
    void operator= (const Singleton&) = delete;

    void FirstTimeInitialize (const char* dir);
    void Initialize (const char* dir);

    const char* GetWorkingDir ();
    const char* GetCommitLogCsv ();
    const char* GetCommitLogMetadata ();
    const char* GetFileArchiverDir ();

private:
    void InitializeStrings (std::string dir);

private:
    std::string workingDir;
    std::string commitLogCsv;
    std::string commitLogMetadata;
    std::string fileArchiverDir;
};

#endif /* WORKING_DIR_INCLUDED */
