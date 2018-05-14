#include "CommitLog.h"
#include "GetTime.h"

#include <WorkingDir.h>
#include <FileArchiver.h>
#include <FileTreeFunctions.h>

#include <Manipulators.h>
#include <sstream>
#include <fstream>
#include <iostream>

using Singleton = CommitLog::Singleton;

Singleton& CommitLog::GetInstance ()
{
    static Singleton singleton;
    return singleton;
}

void Singleton::FirstTimeInitialize ()
{
    int initialCommitId = -1;
    WriteMetadata (0, initialCommitId, initialCommitId);

    std::ofstream ofile (WorkingDir::GetInstance().GetCommitLogCsv ());
    ofile << initialCommitId << ",,,,(){}\n";
    if (!ofile)
    {
        throw std::runtime_error ("CommitLog::FirstTimeInitialize: failed to init csv");
    }
}

int Singleton::PushCommit (const Directory* root, const char* author, const char* descrp)
{
    int id, curr, head;
    ReadMetadata (&id, &curr, &head);
    assert (curr == head);

    std::stringstream os;
    os << id << ',' << descrp << ',' << GetTime << ',' << author << ',' << WriteFileTreeManip (root) << '\n';

    const char* name = WorkingDir::GetInstance().GetCommitLogCsv ();
    std::ofstream ofile (name, std::ios_base::out | std::ios_base::app);
    ofile << os.str ();
    if (!ofile)
    {
        throw std::runtime_error ("CommitLog::Singleton::PushCommit: failed to write commit to csv");
    }

    WriteMetadata (id+1, id, id);
    return id;
}

std::ifstream Singleton::GetTreeHelper (int id, std::string* descrp, std::string* timeThen, std::string* author)
{
    const char* name = WorkingDir::GetInstance().GetCommitLogCsv ();
    std::ifstream ifile (name, std::ios_base::in);

    int currId;
    for (;;)
    {
        ifile >> currId;
        if (!ifile || currId == id) break;
        else ifile >> Ignore ('\n');
    }
    if (ifile.rdstate () & std::ios_base::eofbit)
    {
        throw std::runtime_error ("CommitLog::Singleton::GetHelper: could not find specified commit");
    }

    ifile >> ExactMatch (",");
    ifile >> Scanset ("[^,]", descrp) >> ExactMatch (",");
    ifile >> Scanset ("[^,]", timeThen) >> ExactMatch (",");
    ifile >> Scanset ("[^,]", author) >> ExactMatch (",");

    if (!ifile)
    {
        throw std::runtime_error ("CommitLog::Singleton::GetHelper: error reading csv");
    }
    return ifile;
}

Directory* Singleton::GetTree (int id)
{
    std::ifstream ifile = GetTreeHelper (id);
    return ReadFileTree (ifile);
}

Directory* Singleton::GetHeadTree ()
{
    int head;
    ReadMetadata (NULL, NULL, &head);

    std::ifstream ifile = GetTreeHelper (head);
    return ReadFileTree (ifile);
}

Directory* Singleton::GetCurrTree ()
{
    int curr;
    ReadMetadata (NULL, &curr, NULL);

    std::ifstream ifile = GetTreeHelper (curr);
    return ReadFileTree (ifile);
}

void Singleton::Search (int id, const char* wildcard, SearchFileTreeFromStreamCallback cb)
{
    std::ifstream ifile = GetTreeHelper (id);
    SearchFileTreeFromStream (ifile, wildcard, cb);
}

void Singleton::PrintHistory (std::ostream& os)
{
    const char* name = WorkingDir::GetInstance().GetCommitLogCsv ();
    std::ifstream ifile (name, std::ios_base::in);

    int currId;
    std::string descrp;
    std::string timeThen;
    std::string author;

    os << "Commits:\n";

    while (ifile.peek () != EOF && os)
    {
        ifile >> currId >> ExactMatch (",");
        ifile >> Scanset ("[^,]", &descrp) >> ExactMatch (",");
        ifile >> Scanset ("[^,]", &timeThen) >> ExactMatch (",");
        ifile >> Scanset ("[^,]", &author) >> Ignore ('\n');

        if (ifile)
        {
            os << currId << ": \"" << descrp;
            os << "\" from " << timeThen;
            os << " by " << author << "\n\n";
        }
    }

    if (!os || !ifile)
    {
        throw std::runtime_error ("CommitLog::PrintHistory: could not finish printing history");
    }
}

void Singleton::GetId (int* pCurr, int* pHead)
{
    ReadMetadata (NULL, pCurr, pHead);
}

int Singleton::GetCurrId ()
{
    int curr;
    ReadMetadata (NULL, &curr, NULL);
    return curr;
}

int Singleton::GetHeadId ()
{
    int head;
    ReadMetadata (NULL, NULL, &head);
    return head;
}

bool Singleton::AtHead ()
{
    int curr, head;
    ReadMetadata (NULL, &curr, &head);
    return curr == head;
}

void Singleton::SetCurrId (int curr)
{
    int gen, head;
    ReadMetadata (&gen, NULL, &head);
    WriteMetadata (gen, curr, head);
}

void Singleton::ReadMetadata (int* pNextId, int* pCurr, int* pHead)
{
    const char* name = WorkingDir::GetInstance().GetCommitLogMetadata ();
    std::ifstream ifile (name, std::ios_base::in);

    int ign;
    ifile >> ExactMatch ("next_id: ") >> (pNextId!=NULL ? *pNextId : ign) >> ExactMatch ("\n");
    ifile >> ExactMatch ("curr: ") >> (pCurr!=NULL ? *pCurr : ign) >> ExactMatch ("\n");
    ifile >> ExactMatch ("head: ") >> (pHead!=NULL ? *pHead : ign) >> ExactMatch ("\n");

    if (!ifile)
    {
        throw std::runtime_error ("CommitLog::ReadMetadata: failed reading metadata");
    }
}

void Singleton::WriteMetadata (int nextId, int curr, int head)
{
    std::stringstream ss;
    ss << "next_id: " << nextId << '\n';
    ss << "curr: " << curr << '\n';
    ss << "head: " << head << '\n';

    if (!ss)
    {
        throw std::runtime_error ("CommitLog::WriteMetadata: i/o error");
    }

    const char* name = WorkingDir::GetInstance().GetCommitLogMetadata ();
    std::ofstream ofile (name, std::ios_base::out);

    ofile << ss.str ();
    if (!ofile)
    {
        throw std::runtime_error ("CommitLog::WriteMetadata: i/o error");
    }
}
