#ifndef DIRECTORY_INCLUDED
#define DIRECTORY_INCLUDED

#include "FileBase.h"

#include <FileHashTableNode.h>
#include <FileHashTableSurject.h>
#include <Set.h>
using FileHashTable = Set<FileHashTableNode,FileHashTableSurject>;

class Directory : public FileBase, public FileHashTable
{
public:
    Directory (std::string s);

    Directory (const Directory& rhs) = default;
    Directory (Directory&& rhs) = default;
    Directory& operator= (const Directory& rhs) = default;
    Directory& operator= (Directory&& rhs) = default;

    virtual ~Directory () = default;

    virtual const char* GetName () const;
    virtual bool Identify (const char* s) const;
    virtual const char* GetUID () const;
    virtual FileBase* Clone () const;

private:
    std::string name;

public:
    static constexpr char stringId[] = "directory";
};

#endif /* DIRECTORY_INCLUDED */
