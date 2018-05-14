#include "SearchFileTree.h"

#include <Manipulators.h>
#include <MessageDigest.h>
#include <Automata.h>
#include <WorkingDir.h>
#include <Join.h>

#include <iostream>
#include <cassert>

class SearchFileTreeClass
{
public:
    SearchFileTreeClass (const Automata& automata, std::string& dirname, SearchFileTreeCallback cb);
    void operator() (const Directory* tree);

private:
    const Automata& automata;
    std::string& dirname;
    SearchFileTreeCallback cb;
};

SearchFileTreeClass::SearchFileTreeClass (const Automata& automata, std::string& dirname, SearchFileTreeCallback cb) :
    automata (automata),
    dirname (dirname),
    cb (cb)
{
}

void SearchFileTreeClass::operator() (const Directory* tree)
{
    for (Directory::ConstIterator it = tree->CBegin (); it != tree->CEnd (); ++it)
    {
        if ((*it)->Identify (RegularFile::stringId))
        {
            size_t oldSize = Join (dirname, (*it)->GetName ());
            if (automata.Accepts (dirname.c_str ()))
            {
                cb (it);
            }
            dirname.resize (oldSize);
        }
        else
        {
            size_t oldSize = Join (dirname, (*it)->GetName ());
            if (automata.HasPrefix (dirname.c_str ()))
            {
                (*this)(it->CastToDirectory ());
            }
            dirname.resize (oldSize);
        }
    }
}

void SearchFileTree (const Directory* tree, const char* wildcard, SearchFileTreeCallback cb)
{
    std::string s = WorkingDir::GetInstance().GetWorkingDir ();
    Automata automata (wildcard);
    SearchFileTreeClass(automata, s, cb)(tree);
}

class SearchFileTreeFromStreamClass
{
public:
    SearchFileTreeFromStreamClass (std::istream& is, const Automata& automata, std::string& dirname, bool& found, SearchFileTreeFromStreamCallback cb);
    void operator() ();

private:
    std::istream& is;
    const Automata& automata;
    std::string& dirname;
    bool& found;
    SearchFileTreeFromStreamCallback cb;
};

SearchFileTreeFromStreamClass::SearchFileTreeFromStreamClass (std::istream& is, const Automata& automata, std::string& dirname, bool& found, SearchFileTreeFromStreamCallback cb) :
    is (is),
    automata (automata),
    dirname (dirname),
    found (found),
    cb (cb)
{
    found = false;
}

void SearchFileTreeFromStreamClass::operator() ()
{
    assert (is.exceptions () == (std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit));

    std::string s;
    char delim = is.peek ();

    while (delim != '}')
    {
        if (is.peek () == '(')
        {
            size_t oldSize = dirname.size ();
            is >> ExactMatch ("(") >> Scanset ("[^)]", &dirname, Scanset::Flags::APPEND) >> ExactMatch ("){");

            if (automata.HasPrefix (dirname.c_str ()))
            {
                dirname += '/';
                (*this)();
            }
            else
            {
                int bracketDiff = 1;
                char bracket;
                do
                {
                    is >> Scanset ("[^{}]", nullptr) >> bracket;
                    if (bracket == '{') ++bracketDiff;
                    if (bracket == '}') --bracketDiff;
                }
                while (bracketDiff > 0);
            }

            dirname.resize (oldSize);
        }
        else
        {
            size_t oldSize = dirname.size ();
            is >> Scanset ("[^:]", &dirname, Scanset::Flags::APPEND) >> ExactMatch (":");

            if (automata.Accepts (dirname.c_str ()))
            {
                std::vector <unsigned char> digest;
                is >> ReadDigest (digest);

                cb (dirname.c_str (), digest);
                found = true;
            }
            else
            {
                is >> Scanset ("[^,}]", nullptr);
            }

            dirname.resize (oldSize);
        }

        is >> delim;
    }
}

void SearchFileTreeFromStream (std::istream& is, const char* wildcard, SearchFileTreeFromStreamCallback cb)
{
    std::ios_base::iostate state = is.exceptions ();
    is.exceptions (std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

    std::string s ("");
    Automata automata (wildcard);
    bool found;

    is >> ExactMatch ("(){");
    SearchFileTreeFromStreamClass(is, automata, s, found, cb)();
    is.exceptions (state);

    if (!found)
    {
        std::cerr << "SearchileTreeFromStream: could not match any file with wildcard \'" << wildcard << "\'\n";
        throw std::runtime_error ("SearchileTreeFromStream: failed");
    }
}
