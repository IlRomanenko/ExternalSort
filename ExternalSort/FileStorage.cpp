#include "base.h"
#include "FileStorage.h"

FileStorage::FileStorage(string name, string directory, bool binary, bool is_input)
{
	openmode = (is_input ? ios_base::in : 0) | (is_input ? 0 : ios_base::out) | (binary ? ios_base::binary : 0);
	file_name = name;
	file.open(name, openmode);
}

void FileStorage::write(const void* data, uint size)
{
	const char *str = (char*)data;
	file.write(str, size);
	return;
}

void FileStorage::read(void *data, uint size)
{
	char* stream = (char*)data;
	file.read(stream, size);
	return;
}

bool FileStorage::isEmpty()
{
	return file.eof();
}