#include "fs.h"
#include <stdio.h>
#include <string.h>

static File file_system[NUM_BLOCKS];

void fs_init() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        file_system[i].start_block = 0;
        file_system[i].length = 0;
    }
}

int fs_create_file(const char* name, unsigned int size) {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (file_system[i].length == 0) {
            strncpy(file_system[i].name, name, FILE_NAME_MAX);
            file_system[i].start_block = i;
            file_system[i].length = size;
            char empty_data[BLOCK_SIZE] = {0};
            for (int j = 0; j < size; j++) {
                disk_write(i + j, empty_data);
            }
            return 0;
        }
    }
    return -1;
}

int fs_read_file(const char* name, char* buffer) {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (strncmp(file_system[i].name, name, FILE_NAME_MAX) == 0) {
            for (int j = 0; j < file_system[i].length; j++) {
                disk_read(file_system[i].start_block + j, buffer + (j * BLOCK_SIZE));
            }
            return 0;
        }
    }
    return -1;
}

int fs_write_file(const char* name, const char* data) {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (strncmp(file_system[i].name, name, FILE_NAME_MAX) == 0) {
            for (int j = 0; j < file_system[i].length; j++) {
                disk_write(file_system[i].start_block + j, data + (j * BLOCK_SIZE));
            }
            return 0;
        }
    }
    return -1;
}
