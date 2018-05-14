#include "MergeFileTrees.h"
#include <MessageDigest.h>

#include <iostream>
#include <cstring>

bool MergeFileTreesRec (Directory* tree, Directory* other)
{
    bool changed = false;
    for (Directory::ConstIterator otherIt = other->CBegin (); otherIt != other->CEnd ();)
    {
        Directory::ConstIterator it = tree->CEnd ();
        size_t image = FileHashTableSurject()(*otherIt);
        tree->ForEachInvImg (image, [&it, &otherIt](const Directory::ConstIterator& tmpIt)
        {
            if (strcmp ((*otherIt)->GetName (), (*tmpIt)->GetName ()) == 0)
            {
                it = tmpIt;
            }
        });

        if (it == tree->CEnd ())
        {
            tree->Insert (other->Release (otherIt++));
            changed = true;
        }
        else if (*it != *otherIt)
        {
            tree->Remove (it);
            tree->Insert (other->Release (otherIt++));
            changed = true;
        }
        else if ((*it)->Identify (Directory::stringId))
        {
            FileHashTableNode subTree = tree->Release (it);
            FileHashTableNode subOther = other->Release (otherIt++);
            changed |= MergeFileTreesRec (subTree.CastToDirectory (), subOther.CastToDirectory ());

            tree->Insert (std::move (subTree));
            subOther.Release ();
        }
        else ++otherIt;
    }

    delete other;
    return changed;
}

Directory* MergeFileTrees (Directory* tree, Directory* other)
{
    bool changed = MergeFileTreesRec (tree, other);
    if (!changed)
    {
        std::cerr << "MergeFileTrees: invalid merge -- original tree has not been modified\n";
        throw std::runtime_error ("MergefileTrees: failed");
    }

    return tree;
}
