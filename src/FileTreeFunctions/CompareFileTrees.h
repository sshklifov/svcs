#ifndef COMPARE_FILE_TREES_INCLUDED
#define COMPARE_FILE_TREES_INCLUDED

#include <FileTypes.h>

extern void PatchFileTree (const Directory* tree, const Directory* other);
extern void WriteDiffFileTree (std::ostream& ostr, const Directory* tree, const Directory* other);

class WriteDiffFileTreeManip
{
    friend std::ostream& operator<< (std::ostream& ostr, WriteDiffFileTreeManip&& manip);

public:
    WriteDiffFileTreeManip (const Directory* tree, const Directory* other);

    WriteDiffFileTreeManip () = delete;
    WriteDiffFileTreeManip (const WriteDiffFileTreeManip&) = delete;
    void operator= (const WriteDiffFileTreeManip&) = delete;

private:
    const Directory* tree;
    const Directory* other;
};

std::ostream& operator<< (std::ostream& ostr, WriteDiffFileTreeManip&& manip);

#endif /* COMPARE_FILE_TREES_INCLUDED */
