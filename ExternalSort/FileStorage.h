#pragma once
#include "base.h"
#include "IFileStorage.h"

class FileStorage : public IFileStorage
{
public:
    FileStorage(string name, string directory, file_mode mode)
    {
        openmode = mode;
        file_name = directory + name;
        file.open(file_name, openmode | ios_base::binary);
    }

    void write(const void* data, uint size)
    {
        const char *str = (char*)data;
        file.write(str, size);
        return;
    }

    void read(void *data, uint size)
    {
        char* stream = (char*)data;
        file.read(stream, size);
        return;
    }

    ~FileStorage()
    {
        debugCode(dbg("~FileStorage"));
    }
};

class FormatedFileStorage : public IFormatedFileStorage
{
public:
    FormatedFileStorage(string name, string directory, file_mode mode)
    {
        openmode = mode;
        file_name = directory + name;
        file.open(file_name, openmode);
    }

    ~FormatedFileStorage()
    {
        debugCode(dbg("~FormatedFileStorage()"));
    }
};