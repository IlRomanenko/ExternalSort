#include "base.h"
#include "IExternalAlgorithm.h"
#include <stack>

template <typename T> class ExternalReverse : public IExternalAlgorithm<T>
{
private:
	stack<uint> chunksStack;
	uint current_part_chunk_size, current_chunk_size, chunk_number;
	bool isEndedChunk;

	void beforeWrite(uint chunk_size)
	{
		for (uint i = 0; i < chunk_size / 2; i++)
			swap(chunk[i], chunk[chunk_size - 1 - i]);
	}

	void addedChunk(uint chunk_size)
	{
		chunksStack.push(chunk_number);
		chunk_number++;
	}

	bool chooseElement(T &data)
	{
		if (isEndedChunk)
		{
			if (chunksStack.size() == 0)
				return false;
			isEndedChunk = false;
			chunk_number = chunksStack.top();
			
			uint size = min(blockSize, chunksSizes[chunk_number]);
			chunksSizes[chunk_number] -= size;
			if (chunksSizes[chunk_number] == 0)
				chunksStack.pop();

			readPartOfChunkFromDisk(chunk_number, 0, size);
			current_chunk_size = size;
			current_part_chunk_size = 0;
		}
		
		data = chunk[current_part_chunk_size];
		current_part_chunk_size++;
		
		if (current_part_chunk_size == current_chunk_size)
			isEndedChunk = true;
		return true;
	}

public:
	ExternalReverse(IDataSource<T> *_dataSource, IDataOutSource<T> *_dataOutSource, string _directoryName, uint _blockSize)
		: IExternalAlgorithm(_dataSource, _dataOutSource, _directoryName, _blockSize)
	{
		chunk_number = 0;
		isEndedChunk = true;
	}	

};