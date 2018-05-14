#ifndef WRITE_FILE_TREE_INCLUDED
#define WRITE_FILE_TREE_INCLUDED

#include <FileTypes.h>
#include <ostream>

extern void WriteFileTree (const FileBase* root, std::ostream& os);

class WriteFileTreeManip
{
    friend std::ostream& operator << (std::ostream& os, const WriteFileTreeManip& manip);

public:
    WriteFileTreeManip (const FileBase* root);

    WriteFileTreeManip () = delete;
    WriteFileTreeManip (const WriteFileTreeManip&) = delete;
    void operator= (const WriteFileTreeManip&) = delete;

private:
    const FileBase* root;
};

#endif /* WRITE_FILE_TREE_INCLUDED */
