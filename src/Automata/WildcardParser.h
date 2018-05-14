#ifndef WILDCARD_PARSER_INCLUDED
#define WILDCARD_PARSER_INCLUDED

#include "Automata.h"
#include "ConcreteStates.h"

class WildcardParserBase
{
public:
    virtual StateBase* Parse (const char* s, unsigned& n) = 0;
    virtual ~WildcardParserBase () = default;
};

class WildcardParser
{
public:
    class Singleton;
    static Singleton& GetInstance ();
};

class WildcardParser::Singleton
{
    template <typename T>
    friend class WildcardParserRegisterer;

public:
    Singleton () = default;
    Singleton (const Singleton&) = delete;
    void operator= (const Singleton&) = delete;
    ~Singleton ();

    StateBase* Parse (const char* s, unsigned& n);

private:
    void Register (WildcardParserBase* p);

private:
    static constexpr unsigned CAPACITY = 16;
    WildcardParserBase* registry[CAPACITY];
    unsigned size = 0;
};


template <typename T>
class WildcardParserRegisterer
{
public:
    WildcardParserRegisterer ()
    {
        WildcardParser::GetInstance().Register (new T);
    }
};

#endif /* WILDCARD_PARSER_INCLUDED */
