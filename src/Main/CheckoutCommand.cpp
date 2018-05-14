#include "CommandParser.h"

#include <WorkingDir.h>
#include <CommitLog.h>
#include <FileTreeFunctions.h>

#include <memory>
#include <cstring>
#include <iostream>

class CheckoutCommand : public CommandParserBase
{
public:
    virtual bool Attempt (int argc, const char* const* argv)
    {
        if (strcmp (argv[1], "checkout") == 0)
        {
            if (argc != 4)
            {
                throw std::runtime_error ("CheckoutCommand::Attempt: invalid argument count");
            }

            WorkingDir::GetInstance().Initialize (argv[2]);

            int id;
            if (strcmp (argv[3], "HEAD" )== 0)
            {
                id = CommitLog::GetInstance().GetHeadId ();
            }
            else
            {
                if (sscanf (argv[3], "%d", &id) != 1)
                {
                    std::cerr << "failed to parse " << argv[3] << " as an integer\n";
                    throw std::runtime_error ("CheckoutCommand::Attempt: failed");
                }
            }

            std::unique_ptr<Directory> tree (CommitLog::GetInstance().GetTree (id));
            std::unique_ptr<Directory> fsTree (BuildFileTree ());
            PatchFileTree (tree.get (), fsTree.get ());
            CommitLog::GetInstance().SetCurrId (id);

            return true;
        }
        else return false;
    }

private:
    static Registerer<CheckoutCommand> reg;
};

Registerer<CheckoutCommand> CheckoutCommand::reg;
