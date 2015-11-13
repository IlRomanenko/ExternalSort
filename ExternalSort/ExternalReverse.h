#include "base.h"
#include "IExternalAlgorithm.h"
#include <stack>

template <typename T> class ExternalReverse : public IExternalAlgorithm<T>
{
private:
    stack<uint> chunksStack;
    uint chunk_number;

    void beforeWrite(T *chunk, uint chunk_size)
    {
        for (uint i = 0; i < chunk_size / 2; i++)
            swap(chunk[i], chunk[chunk_size - 1 - i]);
    }

    void prepareAlgorithm()
    {
        uint chunksCount = getChunksCount();
        for (uint i = 0; i < chunksCount; i++)
            chunksStack.push(i);
    }

    bool chooseElement(T& data)
    {
        if (chunksStack.empty())
            return false;
        chunk_number = chunksStack.top();
        getElementFromChunk(chunk_number, data);
        if (isEmptyChunk(chunk_number))
            chunksStack.pop();
        return true;
    }
    
public:
    ExternalReverse(shared_ptr<IDataSource<T> > _dataSource, shared_ptr<IDataOutSource<T> >_dataOutSource, 
        string _directoryName, uint _blockSize)
        : IExternalAlgorithm(_dataSource, _dataOutSource, _directoryName, _blockSize)
    {
    }	

};