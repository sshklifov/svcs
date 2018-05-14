#include "WildcardParser.h"
#include <BracketExpr.h>

#include <iostream>
#include <stdexcept>
#include <cassert>

static bool IsSpecialChar (char c)
{
    switch (c)
    {
    case '?': case '\\': case '[': case ']': case '*': 
        return true;
    }

    return false;
}

WildcardParser::Singleton& WildcardParser::GetInstance ()
{
    static Singleton instance;
    return instance;
}

WildcardParser::Singleton::~Singleton ()
{
    for (unsigned i = 0; i < size; ++ i)
    {
        delete registry[i];
    }
}

StateBase* WildcardParser::Singleton::Parse (const char* s, unsigned& n)
{
    for (unsigned i = 0; i < size; ++i)
    {
        StateBase* res = registry[i]->Parse (s, n);
        if (res != NULL) return res;
    }

    std::cerr << "WildCardParser::Parser: could not parse string: " << s << "\n";
    throw std::runtime_error ("WildcardParser::Parse: failed");
}

void WildcardParser::Singleton::Register (WildcardParserBase* p)
{
    assert (size < CAPACITY);
    registry[size++] = p;
}

class SquareBracketsParser : public WildcardParserBase
{
public:
    virtual StateBase* Parse (const char* s, unsigned& n)
    {
        if (*s != '[') return NULL;

        BracketExpr bracketExpr;
        const char* end = BuildBracketExpr (bracketExpr, s);

        n = end - s;
        return new SquareBracketsState (bracketExpr);
    }

private:
    static WildcardParserRegisterer<SquareBracketsParser> reg;
};

WildcardParserRegisterer<SquareBracketsParser> SquareBracketsParser::reg;

class QuestionMarkParser : public WildcardParserBase
{
public:
    virtual StateBase* Parse (const char* s, unsigned& n)
    {
        if (*s != '?') return NULL;

        n = 1;
        return new QuestionMarkState;
    }

private:
    static WildcardParserRegisterer<QuestionMarkParser> reg;
};

WildcardParserRegisterer<QuestionMarkParser> QuestionMarkParser::reg;

class AsteriskParser : public WildcardParserBase
{
public:
    virtual StateBase* Parse (const char* s, unsigned& n)
    {
        if (*s != '*') return NULL;

        n = 1;
        return new AsteriskState;
    }

private:
    static WildcardParserRegisterer<AsteriskParser> reg;
};

WildcardParserRegisterer<AsteriskParser> AsteriskParser::reg;

class EscapedSingletonParser : public WildcardParserBase
{
public:
    virtual StateBase* Parse (const char* s, unsigned& n)
    {
        if (s[0] != '\\') return NULL;

        if (IsSpecialChar (s[1]))
        {
            n = 2;
            return new SingletonState (s[1]);
        }

        std::cerr << "EscapedSingletonParser::Parse: invalid escape sequence (";
        std::cerr << s[0] << " followed by " << s[1] << ")\n";
        throw std::runtime_error ("EscapedSingletonParser::Parse: failed");

    }

private:
    static WildcardParserRegisterer<EscapedSingletonParser> reg;
};

WildcardParserRegisterer<EscapedSingletonParser> EscapedSingletonParser::reg;

class SingletonParser : public WildcardParserBase
{
public:
    virtual StateBase* Parse (const char* s, unsigned& n)
    {
        if (IsSpecialChar (s[0])) return NULL;

        if (s[0] < 32 || s[0] > 126)
        {
            std::cerr << "SingletonParser::Parse: illegal symbol in wildcard (ASCII: ";
            std::cerr << int (s[0]) << ")\n";
            throw std::runtime_error ("SingletonParser::Parser: failed");
        }

        n = 1;
        return new SingletonState (s[0]);
    }

private:
    static WildcardParserRegisterer<SingletonParser> reg;
};

WildcardParserRegisterer<SingletonParser> SingletonParser::reg;
