#pragma once
#include "base.h"

class IFileStorage
{
public:
	virtual void write(const void *data, uint size) = 0;
	virtual void read(void *data, uint size) = 0;
};

class FileStorage : public IFileStorage
{
private:
	fstream file;
	string file_name;
public:
	FileStorage(string name, string directory, bool binary = false, bool is_input = false)
	{
		file_name = name;
		file.open(name, (is_input ? ios_base::in : 0) | (is_input ? 0 : ios_base::out) | (binary ? ios_base::binary : 0));
	}
	
	void write(const void* data, uint size)
	{
		const char *str = (char*)data;
		file.write(str, size);
		return;
	}

	void read(void *data, uint size)
	{
		char* stream = (char*)data;
		file.read(stream, size);
		return;
	}
	
	void remove()
	{
		file.close();
		std::remove(file_name.c_str());
	}

	~FileStorage()
	{
		file.flush();
		file.close();
	}
};