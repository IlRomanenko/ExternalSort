#pragma once
#include "base.h"
#include "IDataSource.h"
#include "FileStorage.h"
#include <memory>

template <typename StorageType> class IExternalAlgorithm
{
protected:

	//Virtual functions
	virtual void beforeWrite(uint chunk_size) = 0;
	virtual bool choosePart(StorageType &outData) = 0;


	//Base data
	IDataSource<StorageType> *dataSource;
	IDataOutSource<StorageType> *dataOutSource;
	string directoryName;
	uint blockSize;
	vector<IFileStorage*> storages;
	vector<uint> chunkSizes;
	StorageType* chunk;

	//Base functions
	FileStorage* writeChunkToDisk(uint chunk_size)
	{
		FileStorage *storage = new FileStorage(toString(storages.size())+".tmp", directoryName, IFile::file_mode::Write);
		Serializer::SerializeRawData(*storage, chunk, chunk_size);
		return storage;
	}

	uint readPartOfChunkFromDisk(uint file_number, uint offset, uint part_chunk_size)
	{
		uint current = 0;
		while (!storages[file_number]->isEmpty() && current < part_chunk_size)
		{
			Serializer::DeserializeRawData(*storages[file_number], chunk[offset + current]);
			current++;
		}
		return current;
	}

	void prepare()
	{
		uint chunk_size = 0;
		while (!dataSource->isEmpty())
		{
			chunk[chunk_size++] = dataSource->getNext();
			if (chunk_size >= blockSize)
			{
				chunkSizes.push_back(chunk_size);
				beforeWrite(chunk_size);
				storages.push_back(writeChunkToDisk(chunk_size));
				chunk_size = 0;
			}
		}
		if (chunk_size != 0)
		{
			chunkSizes.push_back(chunk_size);
			beforeWrite(chunk_size);
			storages.push_back(writeChunkToDisk(chunk_size));
		}
	}
	
	void mergeChunks()
	{
		StorageType data;
		while (choosePart(data))
		{
			dataOutSource->putNext(data);
		}
	}

public:
	IExternalAlgorithm()
	{
		dataSource = nullptr;
		dataOutSource = nullptr;
		chunk = nullptr;
	}

	IExternalAlgorithm(IDataSource<StorageType> &_dataSource, IDataOutSource<StorageType> &_dataOutSource, string _directoryName, uint _blockSize) :
		directoryName(_directoryName), blockSize(_blockSize) 
	{
		dataSource = &_dataSource;
		dataOutSource = &_dataOutSource;
		chunk = new StorageType[blockSize];
	}	

	void externalWork()
	{
		prepare();
		mergeChunks();
	}

	~IExternalAlgorithm<StorageType>() 
	{
		for (uint i = 0; i < storages.size(); i++)
		{
			storages[i]->~IFileStorage();
			delete storages[i];
		}
		delete chunk;
	}
};