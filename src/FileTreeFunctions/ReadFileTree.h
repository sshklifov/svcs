#ifndef READ_FILE_TREE_INCLUDED
#define READ_FILE_TREE_INCLUDED

#include <FileTypes.h>
#include <istream>

Directory* ReadFileTree (std::istream& is);

class ReadFileTreeManip
{
    friend std::istream& operator>> (std::istream& is, ReadFileTreeManip&& manip);

public:
    ReadFileTreeManip (Directory*& res);

    ReadFileTreeManip () = delete;
    ReadFileTreeManip (const ReadFileTreeManip& res) = delete;
    void operator= (const ReadFileTreeManip& res) = delete;

private:
    Directory*& res;
};

std::istream& operator>> (std::istream& is, ReadFileTreeManip&& manip);

#endif /* READ_FILE_TREE_INCLUDED */
