#pragma once
#include "base.h"
#include "FileStorage.h"
#include <typeinfo>
#include <typeindex>

class Serializer
{
public:

	//BinarySerialization only for simple types such as int([], *, &), double, vector<simple type>, string etc.

	#pragma region BinarySerilization

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
		void SerializeRawData(IFileStorage &file, const T *data, int count)
	{
		uint element_size = sizeof(T);
		file.write(data, count * element_size);
	}

	template <typename T> static
		void SerializeRawData(IFileStorage &file, const T data)
	{
		uint element_size = sizeof(T);
		file.write(&data, element_size);
	}

	template <typename T> static
		void Serialize(IFileStorage &file, const T *data, int count)
	{
		uint element_size = sizeof(T);
		uint size = count;
		file.write(&size, sizeof(uint));
		file.write(data, size * element_size);
	}

	template <typename T> static
		void Serialize(IFileStorage &file, const T data)
	{
		uint size = sizeof(T);
		file.write(&data, size);
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
		file.read(data, element_size * size);
		return size;
	}
	
	template <typename T> static
		void DeserializeRawData(IFileStorage &file, const T *data, int count)
	{
		uint element_size = sizeof(T);
		file.read(data, count * element_size);
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
		memcpy(data, ptr, count * sizeof(T));
		delete ptr;
		return count;
	}
	
	template <typename T> static
		void DeserializeRawData(IFileStorage &file, T &data)
	{
		uint element_size = sizeof(T);
		file.read(&data, element_size);
	}

	template <typename T> static
		void Deserialize(IFileStorage &file, T &data)
	{
		DeserializeRawData(file, data);
	}
	
	#pragma endregion



	//FormatedSerialization for custom types

	#pragma region FormatedSerilization

	template <typename T> static
		void Serialize(IFormatedFileStorage &storage, const T data)
	{
		storage << data << ' ';
	}

	template <typename T> static
		void Serialize(IFormatedFileStorage &storage, const T *data)
	{
		storage << *data << ' ';
	}

	static
		void Serialize(IFormatedFileStorage &storage, const string &str)
	{
		storage << str.size() << ' ' << str << ' ';
	}

	template <typename T> static
		void Serialize(IFormatedFileStorage &storage, const vector<T> &vect)
	{
		storage << vect.size() << ' ';
		for (T value : vect)
			storage << value << ' ';
	}

	template <typename T> static
		void Deserialize(IFormatedFileStorage &storage, T &data)
	{
		storage >> data;
	}

	template <typename T> static
		void Deserialize(IFormatedFileStorage &storage, T *&data)
	{
		if (data == nullptr)
			data = new T;
		storage >> *data;
	}

	template <> static
		void Deserialize(IFormatedFileStorage &storage, string &str)
	{
		uint size = 0;
		storage >> size;
		str.resize(size, '~');
		for (uint i = 0; i < size; i++)
		{
			storage >> str[i];
		}
	}

	template <typename T> static
		void Deserialize(IFormatedFileStorage &storage, vector<T> &vect)
	{
		uint size = 0;
		storage >> size;
		vect.resize(size, T());
		for (uint i = 0; i < size; i++)
			storage >> vect[i];
	}
	#pragma endregion
};