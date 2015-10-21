#pragma once
#include "base.h"
#include "IDataSource.h"
#include "FileStorage.h"

template <typename StorageType> class IExternalAlgorithm
{
private:
    //Private base data
    shared_ptr<IDataSource<StorageType> > dataSource;
    shared_ptr<IDataOutSource<StorageType> > dataOutSource;
    string directoryName;
    vector<unique_ptr<IFileStorage> > storages;
    vector<uint> chunksSizes, chunksPos, totalWrited;
    StorageType* chunk;
    uint maxChunkBlock;
    uint blockSize;

    //Base private functions
    unique_ptr<FileStorage> writeChunkToDisk(uint chunk_size)
    {
        unique_ptr<FileStorage> storage(new FileStorage(toString(storages.size())+".tmp", directoryName, IFile::file_mode::Write));
        Serializer::SerializeRawData(*storage, chunk, chunk_size);
        return storage;
    }

    void addedChunk(uint chunk_size)
    {
        chunksSizes.push_back(chunk_size);
        chunksPos.push_back(0);
        totalWrited.push_back(0);
    }

    void readPartOfChunkFromDisk(uint file_number, uint offset, uint part_chunk_size)
    {
        Serializer::DeserializeRawData(*storages[file_number], (chunk + offset), (int)part_chunk_size);
        return;
    }

    void loadChunk(uint chunk_number)
    {
        uint size = min(maxChunkBlock, chunksSizes[chunk_number] - totalWrited[chunk_number]);
        readPartOfChunkFromDisk(chunk_number, chunk_number * maxChunkBlock, size);
        chunksPos[chunk_number] = chunk_number * maxChunkBlock;
    }

    void prepareChunks()
    {
        uint chunk_size = 0;

        while (!dataSource.get()->isEmpty())
        {
            chunk[chunk_size++] = dataSource.get()->getNext();
            if (chunk_size >= blockSize)
            {
                beforeWrite(chunk, chunk_size);
                storages.push_back(writeChunkToDisk(chunk_size));
                addedChunk(chunk_size);
                chunk_size = 0;
            }
        }
        if (chunk_size != 0)
        {
            beforeWrite(chunk, chunk_size);
            storages.push_back(writeChunkToDisk(chunk_size));
            addedChunk(chunk_size);
        }
        for (uint i = 0; i < storages.size(); i++)
            storages[i]->reopen(IFile::file_mode::Read);
        maxChunkBlock = blockSize / storages.size();

        debugCode(
            for (uint i = 0; i < blockSize; i++)
                chunk[i] = 0;
        );

        for (uint i = 0; i < storages.size(); i++)
        {
            chunksPos[i] = i * maxChunkBlock;
            loadChunk(i);
        }
    }

    void mergeChunks()
    {
        StorageType data;
        while (chooseElement(data))
        {
            dataOutSource.get()->putNext(data);
        }
    }

protected:

    //Virtual functions
    virtual void beforeWrite(StorageType *&chunk, uint chunk_size) = 0;
    virtual bool chooseElement(StorageType &outData) = 0;
    virtual void prepareAlgorithm() = 0;

    //Base functions
    bool isEmptyChunk(uint chunk_number)
    {
        if (chunksSizes[chunk_number] == totalWrited[chunk_number])
            return true;
        return false;
    }

    bool getElementFromChunk(uint chunk_number, StorageType &outData)
    {
        if (isEmptyChunk(chunk_number))
            return false;

        if (chunksPos[chunk_number] / maxChunkBlock == chunk_number + 1)
            loadChunk(chunk_number);

        outData = chunk[chunksPos[chunk_number]];
        chunksPos[chunk_number]++;
        totalWrited[chunk_number]++;
        return true;
    }

    uint getChunksCount()
    {
        return storages.size();
    }

public:
    IExternalAlgorithm(	shared_ptr<IDataSource<StorageType> > _dataSource, shared_ptr<IDataOutSource<StorageType> > _dataOutSource,
        string _directoryName, uint _blockSize) :
    directoryName(_directoryName), blockSize(_blockSize) 
    {
        dataSource = _dataSource;
        dataOutSource = _dataOutSource;
        chunk = new StorageType[blockSize];
    }	

    void externalWork()
    {
        prepareChunks();
        prepareAlgorithm();
        mergeChunks();
    }

    ~IExternalAlgorithm() 
    {
        debugCode(dbg("~IExternalAlgorithm()"));

        for (uint i = 0; i < storages.size(); i++)
        {
            storages[i]->remove();
        }
        delete chunk;
    }
};