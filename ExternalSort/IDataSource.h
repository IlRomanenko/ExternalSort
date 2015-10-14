#pragma once
#include "base.h"

template <typename T> class IDataSource
{
public:
	virtual T getNext() = 0;
	virtual bool isEmpty() = 0;
	virtual ~IDataSource() = 0;
};

template <typename T> class IDataOutSource
{
public:
	virtual T putNext(const T &data) = 0;
	virtual ~IDataOutSource() = 0;
};