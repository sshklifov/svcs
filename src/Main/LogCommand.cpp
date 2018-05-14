#include "CommandParser.h"

#include <WorkingDir.h>
#include <CommitLog.h>

#include <iostream>
#include <cstring>

class LogCommand : public CommandParserBase
{
public:
    virtual bool Attempt (int argc, const char* const* argv)
    {
        if (strcmp (argv[1], "log") == 0)
        {
            if (argc != 3)
            {
                throw std::runtime_error ("LogCommand::Attempt: invalid argument count");
            }

            WorkingDir::GetInstance().Initialize (argv[2]);
            CommitLog::GetInstance().PrintHistory (std::cout);

            return true;
        }
        else return false;
    }

private:
    static Registerer<LogCommand> reg;
};

Registerer<LogCommand> LogCommand::reg;
