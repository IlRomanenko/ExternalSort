#pragma once
#include "base.h"
#include "IDataSource.h"
#include "IFileStorage.h"

template <typename T> class StorageInData : public IDataSource<T>
{
private:
	unique_ptr<IFormatedFileStorage> file;
public:
	StorageInData(IFormatedFileStorage *storage)
	{
		file = unique_ptr<IFormatedFileStorage>(storage);
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
		file->close();
	}
};

template <typename T> class StorageOutData : public IDataOutSource <T>
{
private:
	unique_ptr<IFormatedFileStorage> file;
public:
	StorageOutData(IFormatedFileStorage *storage)
	{
		file = unique_ptr<IFormatedFileStorage>(storage);
	}

	void putNext(const T &data)
	{
		(*file) << data << ' ';
	}

	~StorageOutData()
	{
		file->close();
	}
};
