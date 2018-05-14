#ifndef FILE_HASH_TABLE_SURJECT_INCLUDED
#define FILE_HASH_TABLE_SURJECT_INCLUDED

#include <cstddef>

class FileHashTableNode;

struct FileHashTableSurject
{
    size_t operator() (const FileHashTableNode& node) const;
};

#include "FileHashTableNode.h"

#endif /* FILE_HASH_TABLE_SURJECT_INCLUDED */
