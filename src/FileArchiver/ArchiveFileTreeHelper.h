#ifndef ARCHIVE_FILE_TREE_HELPER_INCLUDED
#define ARCHIVE_FILE_TREE_HELPER_INCLUDED

#include <FileTypes.h>

using ArchiveFileCb = void (*) (const char* file, const std::vector<unsigned char>& digest);

extern void ArchiveFileTreeHelper (const Directory* tree, ArchiveFileCb cb);

#endif /* ARCHIVE_FILE_TREE_HELPER_INCLUDED */
