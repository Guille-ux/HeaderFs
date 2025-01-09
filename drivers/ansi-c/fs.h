#ifndef FS_H
#define FS_H

#include "disk.h"

#define FILE_NAME_MAX 64
#define NUM_BLOCKS (DISK_SIZE / BLOCK_SIZE)

typedef struct {
    char name[FILE_NAME_MAX];
    unsigned int start_block;
    unsigned int length;
} File;

void fs_init();
int fs_create_file(const char* name, unsigned int size);
int fs_read_file(const char* name, char* buffer);
int fs_write_file(const char* name, const char* data);

#endif
