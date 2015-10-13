#include "base.h"
#include "IDataSource.h"

template <typename T> class VectorData : IDataSource<T>
{
private:
	iterator<forward_iterator_tag, T> current, last;
public:
	VectorData(iterator<forward_iterator_tag, T> begin, iterator<forward_iterator_tag, T> end)
	{
		current = begin;
		last = end;
		destructor_func = destructor;
	}
	
	T getNext()
	{
		assert(current == last);
		T data = *current;
		current++;
		return data;
	}

	bool isEmpty()
	{
		return (current == last);
	}

	~VectorData()
	{
	}
};