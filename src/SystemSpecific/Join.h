#ifndef JOIN_INCLUDED
#define JOIN_INCLUDED

#include <string>

inline size_t Join (std::string& dirname, const char* basename)
{
    size_t oldSize = dirname.size ();
    dirname += '/';
    dirname += basename;

    return oldSize;
}

inline size_t Join (std::string& dirname, const char* basename, unsigned n)
{
    size_t oldSize = dirname.size ();
    dirname.append ("/").append (basename, n);

    return oldSize;
}

inline std::string Join (const char* dirname, const char* basename)
{
    std::string res (dirname);
    Join (res, basename);
    return res;
}

#endif /* JOIN_INCLUDED */
