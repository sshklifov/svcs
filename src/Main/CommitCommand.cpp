#include "CommandParser.h"

#include <WorkingDir.h>
#include <CommitLog.h>
#include <FileArchiver.h>
#include <FileTreeFunctions.h>
#include <Join.h>

#include <cstring>
#include <iostream>
#include <memory>

class CommitCommand : public CommandParserBase
{
public:
    virtual bool Attempt (int argc, const char* const* argv)
    {
        if (strcmp (argv[1], "commit") == 0)
        {
            if (argc != 5 && argc != 6)
            {
                throw std::runtime_error ("CommitCommand::Attempt: invalid argument count");
            }
            WorkingDir::GetInstance().Initialize (argv[2]);
            if (!CommitLog::GetInstance().AtHead ())
            {
                throw std::runtime_error ("CommitCommand::Attempt: cannot commit when in detached HEAD state");
            }

            std::unique_ptr<Directory> fsTree;
            const char* author;
            const char* descrp;
            if (argc == 5)
            {
                fsTree.reset (BuildFileTree ());
                author = argv[3];
                descrp = argv[4];
            }
            else
            {
                std::string wildcard = Join (WorkingDir::GetInstance().GetWorkingDir (), argv[3]);
                fsTree.reset (BuildFileTree (wildcard.c_str ()));
                author = argv[4];
                descrp = argv[5];
            }

            std::unique_ptr<Directory> headTree (CommitLog::GetInstance().GetHeadTree ());
            std::unique_ptr<Directory> tree (MergeFileTrees (headTree.get (), fsTree.get ()));

            headTree.release ();
            fsTree.release ();

            FileArchiver::GetInstance().ArchiveFileTree (tree.get ());
            int id = CommitLog::GetInstance().PushCommit (tree.get (), author, descrp);
            std::cout << "Changes commited with id: " << id << "\n";

            return true;
        }
        else return false;
    }

private:
    static Registerer<CommitCommand> reg;
};

Registerer<CommitCommand> CommitCommand::reg;
