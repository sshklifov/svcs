#include "CommandParser.h"

#include <WorkingDir.h>

#include <cstring>
#include <iostream>

class InitCommand : public CommandParserBase
{
public:
    virtual bool Attempt (int argc, const char* const* argv)
    {
        if (strcmp (argv[1], "init") == 0)
        {
            if (argc != 3)
            {
                throw std::runtime_error ("InitCommand::Attempt: invalid argument count");
            }

            WorkingDir::GetInstance().FirstTimeInitialize (argv[2]);
            std::cout << "Repository initialized!\n";

            return true;
        }
        else return false;
    }

private:
    static Registerer<InitCommand> reg;
};

Registerer<InitCommand> InitCommand::reg;
