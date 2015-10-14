#pragma once
#include "base.h"
#include "IDataSource.h"
#include "FileStorage.h"

template <typename StorageType> class IExternalAlgorithm
{
protected:

	//Virtual functions
	virtual void beforeWrite(uint chunk_size) = 0;
	virtual bool chooseElement(StorageType &outData) = 0;
	virtual void addedChunk(uint chunk_size) { }

	//Base data
	unique_ptr<IDataSource<StorageType> > dataSource;
	unique_ptr<IDataOutSource<StorageType> > dataOutSource;
	string directoryName;
	uint blockSize;
	vector<unique_ptr<IFileStorage> > storages;
	vector<uint> chunksSizes;
	StorageType* chunk;

	//Base functions
	unique_ptr<FileStorage> writeChunkToDisk(uint chunk_size)
	{
		unique_ptr<FileStorage> storage(new FileStorage(toString(storages.size())+".tmp", directoryName, IFile::file_mode::Write));
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

	void prepareChunks()
	{
		uint chunk_size = 0;
		while (!dataSource->isEmpty())
		{
			chunk[chunk_size++] = dataSource->getNext();
			if (chunk_size >= blockSize)
			{
				chunksSizes.push_back(chunk_size);
				beforeWrite(chunk_size);
				storages.push_back(writeChunkToDisk(chunk_size));
				addedChunk(chunk_size);
				chunk_size = 0;
			}
		}
		if (chunk_size != 0)
		{
			chunksSizes.push_back(chunk_size);
			beforeWrite(chunk_size);
			storages.push_back(writeChunkToDisk(chunk_size));
			addedChunk(chunk_size);
		}
		for (uint i = 0; i < storages.size(); i++)
			storages[i]->reopen(IFile::file_mode::Read);
	}
	
	void mergeChunks()
	{
		StorageType data;
		while (chooseElement(data))
		{
			dataOutSource->putNext(data);
		}
	}

public:
	IExternalAlgorithm(	IDataSource<StorageType> *_dataSource, IDataOutSource<StorageType> *_dataOutSource,
						string _directoryName, uint _blockSize) :
		directoryName(_directoryName), blockSize(_blockSize) 
	{
		dataSource = unique_ptr<IDataSource<StorageType> >(_dataSource);
		dataOutSource = unique_ptr<IDataOutSource<StorageType> >(_dataOutSource);
		chunk = new StorageType[blockSize];
	}	

	void externalWork()
	{
		prepareChunks();
		mergeChunks();
	}

	~IExternalAlgorithm() 
	{
		for (uint i = 0; i < storages.size(); i++)
		{
			storages[i]->remove();
		}
		delete chunk;
	}
};