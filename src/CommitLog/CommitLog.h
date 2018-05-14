#ifndef COMMIT_LOG_INCLUDED
#define COMMIT_LOG_INCLUDED

#include <FileTypes.h>
#include <FileTreeFunctions.h>

#include <ostream>
#include <string>

class CommitLog
{
public:
    class Singleton;
    static Singleton& GetInstance ();
};

class CommitLog::Singleton
{
public:
    Singleton () = default;
    Singleton (const Singleton&) = delete;
    void operator= (const Singleton&) = delete;

    ~Singleton () = default;

    void FirstTimeInitialize ();

    int PushCommit (const Directory* root, const char* author, const char* descrp);
    void Search (int id, const char* wildcard, SearchFileTreeFromStreamCallback cb);
    void PrintHistory (std::ostream& os);

    Directory* GetTree (int id);
    Directory* GetHeadTree ();
    Directory* GetCurrTree ();

    void GetId (int* pCurr, int* pHead);
    int GetCurrId ();
    int GetHeadId ();
    bool AtHead ();
    void SetCurrId (int curr);

private:
    std::ifstream GetTreeHelper (int id, std::string* descrp = NULL, std::string* timeThen = NULL, std::string* author = NULL);
    void ReadMetadata (int* pCurrId, int* pCurr, int* pHead);
    void WriteMetadata (int currId, int curr, int head);
};

#endif /* COMMIT_LOG_INCLUDED */
