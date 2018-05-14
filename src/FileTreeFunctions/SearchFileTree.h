#ifndef SEARCH_FILE_TREE_INCLUDED
#define SEARCH_FILE_TREE_INCLUDED

#include <FileTypes.h>

using SearchFileTreeCallback = void (*) (const Directory::ConstIterator& it);
extern void SearchFileTree (const Directory* tree, const char* wildcard, SearchFileTreeCallback cb);

using SearchFileTreeFromStreamCallback = void (*) (const char* name, const std::vector<unsigned char>& digest);
extern void SearchFileTreeFromStream (std::istream& is, const char* wildcard, SearchFileTreeFromStreamCallback cb);

#endif /* SEARCH_FILE_TREE_INCLUDED */
