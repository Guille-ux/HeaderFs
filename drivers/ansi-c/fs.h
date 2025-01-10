#ifndef FS_H
#define FS_H

#include "disk.h"

#define PROTECTED_BLOCKS ((uint32_t)SIZE/512)//size of the os. SIZE / 512
#define MAX_FILES 1000//the max number of files that the filesystem can handle
#define MAX_SECTORS ((uint32_t)8574934592) //near 8  gb of memory
#define FILE_NAME_MAX 51
#define NUM_BLOCKS (DISK_SIZE / BLOCK_SIZE)

typedef struct {
    char name[FILE_NAME_MAX];
    unsigned int start_block;
    unsigned int length;
} File;
int search_file();
void fs_init();
int fs_create_file(const char* name, unsigned int size);
int fs_read_file(const char* name, char* buffer);
int fs_write_file(const char* name, const char* data);

#endif
