#ifndef FILE_ARCHIVER_INCLUDED
#define FILE_ARCHIVER_INCLUDED

#include <FileTypes.h>

#include <string>
#include <vector>

class FileArchiver
{
public:
    class Singleton;
    static Singleton& GetInstance ();
};

class FileArchiver::Singleton
{
public:
    Singleton () = default;
    Singleton (const Singleton&) = delete;
    void operator= (const Singleton&) = delete;

    void FirstTimeInitialize ();

    void ArchiveFileTree (const Directory* tree);
    void Restore (const char* where, const std::vector<unsigned char>& digest);

private:
    static std::string GetName (const std::vector<unsigned char>& digest);
    static void ArchiveFileCb (const char* name, const std::vector<unsigned char>& digest);
};

#endif /* FILE_ARCHIVER_INCLUDED */
