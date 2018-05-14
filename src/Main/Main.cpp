#include "CommandParser.h"

int main (int argc, char** argv)
{
    CommandParser::GetInstance().Parse (argc, argv);
}
