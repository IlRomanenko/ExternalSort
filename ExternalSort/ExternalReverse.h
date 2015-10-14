#include "base.h"
#include "IExternalAlgorithm.h"

template <typename T> class ExternalReverse : public IExternalAlgorithm<T>
{
private:

	void beforeWrite(uint chunk_size)
	{
		for (uint i = 0; i < chunk_size / 2; i++)
			swap(chunk[i], chunk[chunk_size - 1 - i]);
	}

	bool choosePart(T &data)
	{
		return false;
	}

public:
	ExternalReverse(IDataSource<T> &_dataSource, IDataOutSource<T> &_dataOutSource, string _directoryName, uint _blockSize)
		: IExternalAlgorithm(_dataSource, _dataOutSource, _directoryName, _blockSize)
	{ }	

};