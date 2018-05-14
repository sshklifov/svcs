#include "CompareFileTrees.h"

#include <WorkingDir.h>
#include <FileArchiver.h>
#include <FileTreeWalk.h>
#include <FilesystemFunctions.h>

template <typename Callback>
class CompareFileTreesClass
{
public:
    CompareFileTreesClass (const Callback& cb);
    void operator() (const Directory* tree, const Directory* other);

private:
    std::string name;
    Callback cb;
};

template <typename Callback>
CompareFileTreesClass<Callback>::CompareFileTreesClass (const Callback& cb) :
    cb (cb)
{
    name = WorkingDir::GetInstance().GetWorkingDir ();
}

template <typename Callback>
void CompareFileTreesClass<Callback>::operator() (const Directory* tree, const Directory* other)
{
    for (Directory::ConstIterator treeIt = tree->CBegin (); treeIt != tree->CEnd (); ++treeIt)
    {
        Directory::ConstIterator otherIt = other->Find (*treeIt);
        if (otherIt == other->CEnd ())
        {
            if ((*treeIt)->Identify (RegularFile::stringId))
            {
                size_t oldSize = Join (name, (*treeIt)->GetName ());
                cb (name.c_str (), treeIt->CastToRegular()->GetDigest ());
                name.resize (oldSize);
            }
            else
            {
                size_t oldSize = Join (name, (*treeIt)->GetName ());
                cb (name.c_str (), treeIt->CastToDirectory ());
                name.resize (oldSize);
            }
        }
        else
        {
            if ((*treeIt)->Identify (Directory::stringId))
            {
                size_t oldSize = Join (name, (*treeIt)->GetName ());
                (*this)(treeIt->CastToDirectory (), otherIt->CastToDirectory ());
                name.resize (oldSize);
            }
        }
    }
}

template <typename Callback>
void CompareFileTrees (const Directory* tree, const Directory* other, const Callback& cb)
{
    (CompareFileTreesClass<Callback>(cb)(tree, other));
}

class PatchCallback
{
public:
    enum class Action {REMOVE, RESTORE};

public:
    PatchCallback (Action action);

    void operator() (const char* name, const std::vector<unsigned char>& digest);
    void operator() (const char* name, const Directory* tree);

private:
    void PatchRec (const Directory* tree);

private:
    Action action;
    std::string name;
};

PatchCallback::PatchCallback (Action action) : action (action)
{
}

void PatchCallback::operator() (const char* name, const std::vector<unsigned char>& digest)
{
    if (action == Action::RESTORE) FileArchiver::GetInstance().Restore (name, digest);
    if (action == Action::REMOVE) RemoveFile (name);
}

void PatchCallback::operator() (const char* s, const Directory* tree)
{
    name = s;
    PatchRec (tree);
}

void PatchCallback::PatchRec (const Directory* tree)
{
    if (action == Action::RESTORE)
    {
        CreateDirectory (name.c_str ());
    }

    for (Directory::ConstIterator it = tree->CBegin (); it != tree->CEnd (); ++it)
    {
        if ((*it)->Identify (RegularFile::stringId))
        {
            size_t oldSize = Join (name, (*it)->GetName ());
            (*this)(name.c_str (), it->CastToRegular()->GetDigest ());
            name.resize (oldSize);
        }
        else
        {
            size_t oldSize = Join (name, (*it)->GetName ());
            PatchRec (it->CastToDirectory ());
            name.resize (oldSize);
        }
    }

    if (action == Action::REMOVE)
    {
        RemoveDirectory (name.c_str ());
    }
}

void PatchFileTree (const Directory* tree, const Directory* other)
{
    CompareFileTrees (other, tree, PatchCallback (PatchCallback::Action::REMOVE));
    CompareFileTrees (tree, other, PatchCallback (PatchCallback::Action::RESTORE));
}

class WriteDiffCallback
{
public:
    WriteDiffCallback (std::ostream& ostr, bool& wroteSomething);

    void operator() (const char* name, const std::vector<unsigned char>&);
    void operator() (const char* name, const Directory*);

private:
    void Write (const char* name);

private:
    std::ostream& ostr;
    bool& wroteSomething;
};

WriteDiffCallback::WriteDiffCallback (std::ostream& ostr, bool& wroteSomething) :
    ostr (ostr), wroteSomething (wroteSomething)
{
    wroteSomething = false;
}

void WriteDiffCallback::operator() (const char* name, const std::vector<unsigned char>&)
{
    Write (name);
}

void WriteDiffCallback::operator() (const char* name, const Directory*)
{
    Write (name);
}

void WriteDiffCallback::Write (const char* name)
{
    if (!wroteSomething)
    {
        ostr << "Not commited:\n";
        wroteSomething = true;
    }

    ostr << "    " << name << "\n";
}

WriteDiffFileTreeManip::WriteDiffFileTreeManip (const Directory* tree, const Directory* other) :
    tree (tree),
    other (other)
{
}

std::ostream& operator<< (std::ostream& ostr, WriteDiffFileTreeManip&& manip)
{
    WriteDiffFileTree (ostr, manip.tree, manip.other);
    return ostr;
}

void WriteDiffFileTree (std::ostream& ostr, const Directory* tree, const Directory* other)
{
    std::string name = WorkingDir::GetInstance().GetWorkingDir ();

    bool wroteSomething;
    CompareFileTrees (other, tree, WriteDiffCallback (ostr, wroteSomething));

    if (!wroteSomething)
    {
        ostr << "No new files to commit\n";
    }
}
