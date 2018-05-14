#include "CommandParser.h"

#include <WorkingDir.h>
#include <CommitLog.h>
#include <FileArchiver.h>
#include <FileTreeFunctions.h>
#include <FilesystemFunctions.h>
#include <MessageDigest.h>

#include <iostream>
#include <cstring>

static void RevertCallback (const char* relName, const std::vector<unsigned char>& digest)
{
    std::string name = WorkingDir::GetInstance().GetWorkingDir ();
    size_t oldSize = Join (name, relName);
    if (FileExists (name.c_str ()) && CompareDigests (DigestFile (name.c_str ()), digest))
    {
        return;
    }

    size_t pos = oldSize;
    while ((pos=name.find_first_of ('/', pos+1)) != std::string::npos)
    {
        name[pos] = '\0';
        if (!DirectoryExists (name.c_str ()))
        {
            CreateDirectory (name.c_str ());
        }
        name[pos] = '/';
    }

    FileArchiver::GetInstance().Restore (name.c_str (), digest);
    std::cout << "Reverted file: " << name.c_str () << "\n";
}

class RevertCommand : public CommandParserBase
{
public:
    virtual bool Attempt (int argc, const char* const* argv)
    {
        if (strcmp (argv[1], "revert") == 0)
        {
            if (argc != 5)
            {
                throw std::runtime_error ("RevertCommand::Attempt: invalid argument count");
            }

            WorkingDir::GetInstance().Initialize (argv[2]);

            int id;
            if (sscanf (argv[3], "%d", &id) != 1)
            {
                std::cerr << "failed to parse " << argv[3] << " as an integer\n";
                throw std::runtime_error ("RevertCommand::Attempt: failed");
            }
            CommitLog::GetInstance().Search (id, argv[4], RevertCallback);

            return true;
        }
        else return false;
    }

private:
    static Registerer<RevertCommand> reg;
};

Registerer<RevertCommand> RevertCommand::reg;
