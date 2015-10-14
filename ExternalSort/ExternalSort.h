#pragma once
#include "base.h"
#include "IDataSource.h"
#include "FileStorage.h"

template <typename T> class IExternalAlgorithm
{
protected:
	virtual void beforeWrite(uint chunk_size) = 0;
	virtual bool choosePart(T &outData) = 0;
private:
	IDataSource<T> dataSource;
	IDataOutSource<T> dataOutSorce;
	string directoryName;
	uint blockSize;
	vector<IFileStorage> storages;
	vector<uint> chunkSizes;
	T* chunk;

	void writeChunkToDisk(uint chunk_size)
	{
		FileStorage storage(toString(storages.size())+".tmp", directoryName);
		Serializer::SerializeRawData(storage, chunk, chunk_size);
	}

	uint readPartOfChunkFromDisk(uint file_number, uint offset, uint part_chunk_size)
	{
		uint current = 0;
		while (!storages[file_number].isEmpty() && current < part_chunk_size)
		{
			Serializer::DeserializeRawData(storages[file_number], chunk[offset + current]);
			current++;
		}
		return current;
	}

	void prepare()
	{
		uint chunk_size = 0;
		while (!dataSource.isEmpty())
		{
			chunk[chunk_size++] = dataSource.getNext();
			if (chunk_size >= blockSize)
			{
				chunkSizes.push_back(chunk_size);
				beforeWrite(chunk_size);
				writeChunkToDisk(chunk_size);
				chunk_size = 0;
			}
		}
		if (chunk_size != 0)
		{
			chunkSizes.push_back(chunk_size);
			beforeWrite(chunk_size);
			writeChunkToDisk(chunk_size);
		}
	}

	void mergeChunks()
	{
		T data;
		while (choosePart(data))
		{
			dataOutSorce << data;
		}
	}

public:
	IExternalAlgorithm(IDataSource<T> _dataSource, IDataOutSource<T> _dataOutSource, string _directoryName, uint _blockSize) :
		dataSource(_dataSource), dataOutSource(_dataOutSource), directoryName(_directoryName), blockSize(_blockSize) 
	{
		chunk = new T[blockSize];
	}	

	void externalWork()
	{
		prepare();
		mergeChunks();
	}

	~IExternalAlgorithm() 
	{
		delete chunk;
	}
};