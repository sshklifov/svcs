#include "CommandParser.h"

#include <WorkingDir.h>
#include <CommitLog.h>
#include <FileTreeFunctions.h>

#include <memory>
#include <iostream>
#include <cstring>

class StatusCommand : public CommandParserBase
{
public:
    virtual bool Attempt (int argc, const char* const* argv)
    {
        if (strcmp (argv[1], "status") == 0)
        {
            if (argc != 3)
            {
                throw std::runtime_error ("StatusCommand::Attempt: invalid argument count");
            }

            WorkingDir::GetInstance().Initialize (argv[2]);

            int id = CommitLog::GetInstance().GetCurrId ();
            std::unique_ptr<Directory> tree (CommitLog::GetInstance().GetTree (id));
            std::unique_ptr<Directory> fsTree (BuildFileTree ());

            std::cout << "Current commit: " << id << "\n";
            std::cout << WriteDiffFileTreeManip (tree.get (), fsTree.get ());

            return true;
        }
        else return false;
    }

private:
    static Registerer<StatusCommand> reg;
};

Registerer<StatusCommand> StatusCommand::reg;

