#include "FileHashTableSurject.h"
#include <HashFunctions.h>

#include <FileTypes.h>

size_t FileHashTableSurject::operator() (const FileHashTableNode& node) const
{
    return HashString (node->GetName ());
}
