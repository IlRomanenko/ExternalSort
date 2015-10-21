#pragma once
#include "base.h"
#include "IDataSource.h"
#include "IFileStorage.h"

template <typename T> class StorageInData : public IDataSource<T>
{
private:
    unique_ptr<IFormatedFileStorage> file;
public:
    StorageInData(unique_ptr<IFormatedFileStorage> storage)
    {
        file.swap(storage);
    }

    T getNext()
    {
        T data;
        (*file) >> data;
        return data;
    }

    bool isEmpty()
    {
        return file->isEmpty();
    }

    ~StorageInData()
    {
        debugCode(dbg("~StorageInData()"));
    }
};

template <typename T> class StorageOutData : public IDataOutSource <T>
{
private:
    unique_ptr<IFormatedFileStorage> file;
public:
    StorageOutData(unique_ptr<IFormatedFileStorage> storage)
    {
        file.swap(storage);
    }

    void putNext(const T &data)
    {
        (*file) << data << ' ';
    }

    ~StorageOutData()
    {
        debugCode(dbg("~StorageOutData()"));
    }
};
