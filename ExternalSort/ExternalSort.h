#pragma once
#include "base.h"
#include "IExternalAlgorithm.h"
#include <queue>

template <typename T, typename Pred = less<T>, typename EqualPred = equal_to<T> > class ExternalSort : public IExternalAlgorithm<T>
{
private:
    Pred comparator;

    template <typename T, typename Pred, typename EqualPred> class T_Pair_Pred
    {
        Pred comp;
        EqualPred equalComp;
    public:
        T_Pair_Pred(Pred _comp = less<T>(), EqualPred _equal = equal_to<T>())
        {
            comp = _comp;
            equalComp = _equal;
        }
        bool operator() (const pair<T, uint>& left, const pair<T, uint>& right) const
        {	
            return !(comp(left.first, right.first) || (equalComp(left.first, right.first) && left.second < right.second));
        }
    };

    typedef priority_queue<pair<T, uint>, vector<pair<T, uint> >, T_Pair_Pred<T, Pred, EqualPred> >  pair_priority_queue;

    unique_ptr<pair_priority_queue> pr_queue;

    void beforeWrite(T *chunk, uint chunk_size)
    {
        sort(chunk, chunk + chunk_size, comparator);
    }

    void prepareAlgorithm()
    {
        uint chunksCount = getChunksCount();
        T data;
        for (uint i = 0; i < chunksCount; i++)
        {
            getElementFromChunk(i, data);
            pr_queue->push(make_pair(data, i));
        }
    }

    bool chooseElement(T& data)
    {
        if (pr_queue->empty())
            return false;

        pair<T, uint> temp_pair;
        uint chunk_number;

        temp_pair = pr_queue->top();
        pr_queue->pop();

        data = temp_pair.first;
        chunk_number = temp_pair.second;

        if (!isEmptyChunk(chunk_number))
        {
            getElementFromChunk(chunk_number, temp_pair.first);
            pr_queue->push(make_pair(temp_pair.first, chunk_number));
        }
        return true;
    }

public:
    ExternalSort(shared_ptr<IDataSource<T> > _dataSource, shared_ptr<IDataOutSource<T> > _dataOutSource, string _directoryName, 
        uint _blockSize, Pred _comparator = less<T>(), EqualPred _equalPred = equal_to<T>())
        : IExternalAlgorithm(_dataSource, _dataOutSource, _directoryName, _blockSize)
    {
        pr_queue = unique_ptr<pair_priority_queue>(
            new pair_priority_queue(
                T_Pair_Pred<T, Pred, EqualPred>(_comparator, _equalPred)
                )
            );
        comparator = _comparator;
    }	
};
