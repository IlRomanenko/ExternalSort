#pragma once
#include "base.h"
#include "FileStorage.h"
#include <typeinfo>
#include <typeindex>

//Only for simple types such as int([], *, &), double, vector<simple type>, string etc.
class Serializer
{
public:
	static
		void Serialize(IFileStorage &file, const string &str)
	{
		Serialize(file, str.c_str(), str.size());
	}

	template <typename T> static 
		void Serialize(IFileStorage &file, const vector<T> &v)
	{
		uint size = v.size();
		uint element_size = sizeof(T);
		file.write(&size, sizeof(uint));
		for (uint i = 0; i < size; i++)
			file.write(&v[i], element_size);
	}
	
	template <typename T, size_t N> static
		void Serialize(IFileStorage &file, const T (&data)[N])
	{
		Serialize(file, data, N);
	}

	template <typename T> static
		void Serialize(IFileStorage &file, const T *data, int count)
	{
		uint element_size = sizeof(T);
		uint size = count;
		file.write(&size, sizeof(uint));
		for (uint i = 0; i < size; i++)
			file.write(&data[i], element_size);
	}

	template <typename T> static
		void Serialize(IFileStorage &file, const T data)
	{
		const T *ptr = &data;
		Serialize(file, ptr, 1);
	}

	static
		void Deserialize(IFileStorage &file, string &str)
	{
		char *ptr = nullptr;
		uint size = Deserialize(file, ptr);
		str = string(ptr, size);
		delete ptr;
	}

	template <typename T> static
		void Deserialize(IFileStorage &file, vector<T> &v)
	{
		uint size = 0;
		uint element_size = sizeof(T);
		file.read(&size, sizeof(uint));
		for (uint i = 0; i < size; i++)
			file.read(&v[i], element_size);
	}

	template <typename T> static
		uint Deserialize(IFileStorage &file, T *&data)
	{
		uint element_size = sizeof(T);
		uint size = 0;
		file.read(&size, sizeof(uint));
		if (data == nullptr)
			data = new T[size];
		for (uint i = 0; i < size; i++)
			file.read(&data[i], element_size);
		return size;
	}
	
	
	template <typename T, size_t N> static
		uint Deserialize(IFileStorage &file, T (&data)[N])
	{
		T* ptr = nullptr;
		uint count = Deserialize(file, ptr);
		if (count > N)
		{
			delete ptr;
			throw exception();
		}
		for (uint i = 0; i < count; i++)
			data[i] = *(ptr + i);
		delete ptr;
		return count;
	}
	
	template <typename T> static
		void Deserialize(IFileStorage &file, T &data)
	{
		T* ptr = nullptr;
		Deserialize(file, ptr);
		data = *ptr;
		delete ptr;
	}
};