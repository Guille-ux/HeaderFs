#include "disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *disk;

void disk_init() {
    disk = fopen(DISK_FILE, "w+b");
    if (disk == NULL) {
        printf("Error opening the virtual disk\n");
        exit(1);
    }
    char zero[BLOCK_SIZE] = {0};
    for (int i = 0; i < DISK_SIZE / BLOCK_SIZE; i++) {
        disk_write(i, zero);
    }
}

void clean_disk() {
    char zero[BLOCK_SIZE] = {0};
    for (int i = 0; i < DISK_SIZE / BLOCK_SIZE; i++) {
        disk_write(i, zero);
    }
} 

void disk_read(unsigned int sector, char* buffer) {
    fseek(disk, sector * BLOCK_SIZE, SEEK_SET);
    fread(buffer, BLOCK_SIZE, 1, disk);
}

void disk_write(unsigned int sector, const char* buffer) {
    fseek(disk, sector * BLOCK_SIZE, SEEK_SET);
    fwrite(buffer, BLOCK_SIZE, 1, disk);
}
