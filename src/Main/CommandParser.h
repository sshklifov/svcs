#ifndef COMMAND_PARSER_INCLUDED
#define COMMAND_PARSER_INCLUDED

#include <string>

class CommandParserBase
{
public:
    virtual bool Attempt (int argc, const char* const* argv) = 0;
    virtual ~CommandParserBase () = default;
};

class CommandParser
{
public:
    class Singleton;
    static Singleton& GetInstance ();
};

class CommandParser::Singleton
{
public:
    Singleton () = default;

    Singleton (const Singleton&) = delete;
    void operator= (const Singleton&) = delete;

    ~Singleton ();

    void Register (CommandParserBase* p);
    void Parse (int argc, const char* const* argv);

private:
    static constexpr unsigned capacity = 10;
    CommandParserBase* arr[capacity];
    unsigned size = 0;
};

template <typename T>
class Registerer
{
public:
    Registerer () { CommandParser::GetInstance().Register (new T); }
};

#endif /* COMMAND_PARSER_INCLUDED */
