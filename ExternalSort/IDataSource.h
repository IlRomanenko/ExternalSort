#pragma once
#include "base.h"

template <typename T> class IDataSource
{
public:
    virtual T getNext() = 0;
    virtual bool isEmpty() = 0;
};

template <typename T> class IDataOutSource
{
public:
    virtual void putNext(const T &data) = 0;
};