#include "FileArchiver.h"
#include "ArchiveFileTreeHelper.h"

#include <WorkingDir.h>
#include <MessageDigest.h>

#include <sstream>
#include <iostream>
#include <fstream>

using Singleton = FileArchiver::Singleton;

Singleton& FileArchiver::GetInstance ()
{
    static Singleton instance;
    return instance;
}

void Singleton::FirstTimeInitialize ()
{
}

void Singleton::ArchiveFileTree (const Directory* tree)
{
    ArchiveFileTreeHelper (tree, ArchiveFileCb);
};

void Singleton::Restore (const char* where, const std::vector<unsigned char>& digest)
{
    std::ifstream ifile (GetName(digest).c_str (), std::ios_base::in);
    if (!ifile)
    {
        std::cerr << "FileArchiver::Restore: could not open input file with hash ";
        std::cerr << WriteDigest (digest) << "\n";
        throw std::runtime_error ("FileArchiver:Restore: failed");
    }

    std::ofstream ofile (where, std::ios_base::out | std::ios_base::trunc);
    if (!ofile)
    {
        std::cerr << "FileArchiver::Restore: could not open output file " << where << "\n";
        throw std::runtime_error ("FileArchiver:Restore: failed");
    }

    if (ifile.peek () != EOF)
    {
        ofile << ifile.rdbuf ();
    }
    if (!ofile)
    {
        throw std::runtime_error ("FileArchiver::Restore: i/o error");
    }
}

std::string Singleton::GetName (const std::vector<unsigned char>& digest)
{
    std::ostringstream oss;
    const char* name = WorkingDir::GetInstance().GetFileArchiverDir ();
    oss << name << '/' << WriteDigest (digest);

    return oss.str ();
}

void Singleton::ArchiveFileCb (const char* name, const std::vector<unsigned char>& digest)
{
    std::ifstream ifile (name, std::ios_base::in);
    if (!ifile)
    {
        std::cerr << "FileArchiver::ArchiveFileCb: failed to open input file" << name << "\n";
        throw std::runtime_error ("FileArchiver:ArchiveFileCb: failed");
    }

    std::ofstream ofile (GetName(digest).c_str (), std::ios_base::out);
    if (ofile.good () && ifile.peek () != EOF)
    {
        ofile << ifile.rdbuf ();
    }
    if (!ofile)
    {
        throw std::runtime_error ("FileArchiver::ArchiveFileCb: i/o error");
    }
}
