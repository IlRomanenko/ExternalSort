#pragma once
#include "base.h"

template <typename T> class IDataSource
{
public:
	virtual T getNext() = 0;
	virtual bool isEmpty() = 0;
	virtual ~IDataSource() = 0;
};