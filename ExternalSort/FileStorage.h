#pragma once
#include "base.h"
#include "IFileStorage.h"

class FileStorage : public IFileStorage
{
public:
	FileStorage(string name, string directory, file_mode mode)
	{
		openmode = mode;
		file_name = directory + name;
		file.open(name, openmode);
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

	void reopen()
	{
		file.close();
		file.open(file_name, ios_base::in | ios_base::binary);
	}

	~FileStorage()
	{
		file.flush();
		file.close();
	}
};

class FormatedFileStorage : public IFormatedFileStorage
{
public:
	FormatedFileStorage(string name, string directory, file_mode mode)
	{
		openmode = mode;
		file_name = directory + name;
		file.open(name, openmode);
	}

	~FormatedFileStorage()
	{
		file.flush();
		file.close();
	}
};