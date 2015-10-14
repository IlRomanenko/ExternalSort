#include "base.h"
#include "IDataSource.h"
#include "IFileStorage.h"

template <typename T> class StorageInData : public IDataSource<T>
{
private:
	IFormatedFileStorage &file;
public:
	StorageInData(IFormatedFileStorage &storage)
	{
		file = storage;
	}
	
	T getNext()
	{
		assert(isEmpty());
		T data = default(T);
		file >> data;
		return data;
	}

	bool isEmpty()
	{
		return file.isEmpty();
	}

	~StorageInData()
	{
		file.close();
	}
};

template <typename T> class StorageOutData : public IDataOutSource <T>
{
private:
	IFormatedFileStorage &file;
public:
	StorageOutData(IFormatedFileStorage &storage)
	{
		file = storage;
	}

	void putNext(const T &data)
	{
		file << data << ' ';
	}

	~StorageOutData()
	{
		file.close();
	}
};
