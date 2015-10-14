#pragma once
#include "base.h"
#include "IDataSource.h"
#include "IFileStorage.h"

template <typename T> class StorageInData : public IDataSource<T>
{
private:
	IFormatedFileStorage *file;
public:
	StorageInData<T>(IFormatedFileStorage &storage)
	{
		file = &storage;
	}
	
	T getNext()
	{
		assert(isEmpty());
		T data;
		(*file) >> data;
		return data;
	}

	bool isEmpty()
	{
		return file->isEmpty();
	}

	~StorageInData<T>()
	{
		file->close();
	}
};

template <typename T> class StorageOutData : public IDataOutSource <T>
{
private:
	IFormatedFileStorage *file;
public:
	StorageOutData<T>(IFormatedFileStorage &storage)
	{
		file = &storage;
	}

	void putNext(const T &data)
	{
		(*file) << data << ' ';
	}

	~StorageOutData<T>()
	{
		file->close();
	}
};
