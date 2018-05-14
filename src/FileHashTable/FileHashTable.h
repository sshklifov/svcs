#ifndef FILE_HASH_TABLE_INCLUDED
#define FILE_HASH_TABLE_INCLUDED

#include "FileHashTableNode.h"
#include "FileHashTableSurject.h"
#include <Set.h>

using FileHashTable = Set<FileHashTableNode,FileHashTableSurject>;

#endif /* FILE_HASH_TABLE_INCLUDED */
