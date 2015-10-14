#pragma once
#include "base.h"
#include <cassert>

class IFile
{
protected:
	fstream file;
	string file_name;
	ios_base::open_mode openmode;
public:
	enum file_mode { Read = ios_base::in, Write = ios_base::out, ReadWrite = ios_base::in | ios_base::out };

	virtual void reopen(file_mode mode)
	{
		close();
		openmode = mode;
		file.open(file_name, openmode);
	}

	virtual bool isEmpty()
	{
		return file.eof();
	}
	
	virtual void close() 
	{
		file.flush();
		file.close();
	}

	virtual void remove()
	{
		file.close();
		std::remove(file_name.c_str());
	}

	virtual ~IFile()
	{
		file.flush();
		file.close();
	}
};

class IFileStorage : public IFile
{
public:
	virtual void write(const void *data, uint size) = 0;
	virtual void read(void *data, uint size) = 0;
	virtual ~IFileStorage()
	{
		file.flush();
		file.close();
	}
};

class IFormatedFileStorage : public IFile
{
public:
	template <typename T> ostream& operator<<(T data)
	{
		return file << data;
	}
	template <typename T> istream& operator>>(T &data)
	{
		return file >> data;
	}
	virtual ~IFormatedFileStorage()
	{
		file.flush();
		file.close();
	}
};
