#include "BuildFileTree.h"

#include <MessageDigest.h>
#include <FileTreeWalk.h>
#include <Automata.h>
#include <WorkingDir.h>
#include <HashFunctions.h>

#include <memory>

class BuildFileTreeClass
{
public:
    BuildFileTreeClass (const Automata& automata, std::string& dirname);
    Directory* operator() (FileTreeWalk it, const char* name);

private:
    FileTreeWalk it;
    const Automata& automata;
    std::string& dirname;
    const char* name;
};

BuildFileTreeClass::BuildFileTreeClass (const Automata& automata, std::string& dirname) :
    automata (automata),
    dirname (dirname)
{
}

Directory* BuildFileTreeClass::operator() (FileTreeWalk it, const char* name)
{
    std::unique_ptr<Directory> res (new Directory (name));
    while (!it.IsFinished ())
    {
        if (!it.IsDirectory ())
        {
            size_t oldSize = Join (dirname, it.GetBasename ());
            if (automata.Accepts (dirname.c_str ()))
            {
                std::vector<unsigned char> digest = DigestFile (dirname.c_str ());
                res->Insert (new RegularFile (it.GetBasename (), std::move (digest)));
            }
            dirname.resize (oldSize);
        }
        else
        {
            size_t oldSize = Join (dirname, it.GetBasename ());
            if (automata.HasPrefix (dirname.c_str ()))
            {
                res->Insert ((*this)(FileTreeWalk (dirname.c_str ()), it.GetBasename ()));
            }
            dirname.resize (oldSize);
        }

        it.Advance ();
    }

    return res.release ();
}

Directory* BuildFileTree (const char* wildcard)
{
    std::string dirname = WorkingDir::GetInstance().GetWorkingDir ();
    Automata automata (wildcard);
    return BuildFileTreeClass(automata, dirname)(FileTreeWalk (dirname), "");
};
