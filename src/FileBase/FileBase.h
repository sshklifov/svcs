#ifndef FILE_BASE_INCLUDED
#define FILE_BASE_INCLUDED

class FileBase
{
public:
    virtual ~FileBase () = default;

    virtual const char* GetName () const = 0;
    virtual FileBase* Clone () const = 0;
    virtual bool Identify (const char* s) const = 0;
    virtual const char* GetUID () const = 0;
};

#endif /* FILE_BASE_INCLUDED */
