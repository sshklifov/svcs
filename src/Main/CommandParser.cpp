#include "CommandParser.h"

#include <cassert>
#include <iostream>

using Singleton = CommandParser::Singleton;

Singleton& CommandParser::GetInstance ()
{
    static Singleton instance;
    return instance;
}

Singleton::~Singleton ()
{
    for (unsigned i = 0; i < size; ++i)
    {
        delete arr[i];
    }
}

void Singleton::Register (CommandParserBase* p)
{
    assert (size < capacity);
    arr[size++] = p;
}

void Singleton::Parse (int argc, const char* const* argv)
{
    if (argc < 2)
    {
        std::cerr << "CommandParser::Parse: specify command when invoking svcs\n";
        throw std::runtime_error ("CommandParser::Parse: failed");
    }

    for (unsigned i = 0; i < size; ++i)
    {
        if (arr[i]->Attempt (argc, argv)) return;
    }

    std::cerr << "CommandParser::Parse: failed to parse command \'" << argv[1] << "\'\n";
    throw std::runtime_error ("CommandParser::Parse: failed");
}

