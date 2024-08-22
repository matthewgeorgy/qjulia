#ifndef MG_FILE_H
#define MG_FILE_H

#include "mg_defs.h"

typedef enum _TAG_mg_file_access
{
    MG_FILE_ACCESS_READ = GENERIC_READ,
    MG_FILE_ACCESS_WRITE = GENERIC_WRITE,
    MG_FILE_ACCESS_READWRITE = GENERIC_READ | GENERIC_WRITE
} mg_file_access_e;

typedef enum _TAG_mg_filemap_access
{
	MG_FILEMAP_ACCESS_READ = FILE_MAP_READ,
	MG_FILEMAP_ACCESS_WRITE = FILE_MAP_WRITE,
	MG_FILEMAP_ACCESS_READWRITE = FILE_MAP_ALL_ACCESS
} mg_filemap_access_e;

typedef enum _TAG_mg_file_ptr
{
	MG_FILE_PTR_BEGIN = FILE_BEGIN,
	MG_FILE_PTR_CURRENT = FILE_CURRENT,
	MG_FILE_PTR_END = FILE_END
} mg_file_ptr_e;

mg_handle_t		mg_file_create(char *filename, mg_file_access_e access);
mg_handle_t		mg_file_open(char *filename, mg_file_access_e access);
b32				mg_file_close(mg_handle_t file);
b32				mg_file_read(mg_handle_t file, void *buffer, u32 byte_cnt);
b32				mg_file_write(mg_handle_t file, void *buffer, u32 byte_cnt);
u64				mg_file_size(mg_handle_t file);
u32				mg_file_ptr_set(mg_handle_t file, u32 distance, mg_file_ptr_e method);
u32				mg_file_ptr_get(mg_handle_t file);
char			**mg_file_string(char *filename, s32 len);

mg_handle_t		mg_filemap_create(mg_handle_t handle, u32 size, char *name);
mg_handle_t		mg_filemap_open(char *name, mg_filemap_access_e access);
b32				mg_filemap_close(mg_handle_t handle);
void			*mg_filemap_map(mg_handle_t filemap, mg_filemap_access_e access, u32 size);
b32				mg_filemap_unmap(void *ptr);



#ifdef MG_IMPL

mg_handle_t
mg_file_create(char *filename,
			   mg_file_access_e access)
{
	return (CreateFileA(filename, access, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
}

mg_handle_t
mg_file_open(char *filename,
			 mg_file_access_e access)
{
	return (CreateFileA(filename, access, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
}

b32
mg_file_close(mg_handle_t file)
{
	return (CloseHandle(file));
}

b32
mg_file_read(mg_handle_t file,
			 void *buffer,
			 u32 byte_cnt)
{
	b32		ret;
	u32		read;


	ret =  ReadFile(file, buffer, byte_cnt, (LPDWORD)&read, NULL);

	return (ret);
}

b32
mg_file_write(mg_handle_t file,
			  void *buffer,
			  u32 byte_cnt)
{
	b32		ret;
	u32		written;


	ret = WriteFile(file, buffer, byte_cnt, (LPDWORD)&written, NULL);

	return (ret);
}

u64
mg_file_size(mg_handle_t file)
{
	LARGE_INTEGER		size;


	GetFileSizeEx(file, &size);

	return (size.QuadPart);
}

u32				
mg_file_ptr_set(mg_handle_t file,
				u32 distance, 
				mg_file_ptr_e method)
{
	return (SetFilePointer(file, distance, NULL, method));
}

u32				
mg_file_ptr_get(mg_handle_t file)
{
	return (SetFilePointer(file, 0, NULL, FILE_CURRENT));
}

#endif // MG_IMPL

#endif // MG_FILE_H

