/* This program is free software: you can redistribute it and/or modify
/* it under the terms of the GNU General Public License as published by
/* the Free Software Foundation, either version 3 of the License, or
/* (at your option) any later version.
/* This program is distributed in the hope that it will be useful,
/* but WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/* GNU General Public License for more details.
/* You should have received a copy of the GNU General Public License
/* along with this program. If not, see <https://www.gnu.org/licenses/>.
/* Copyright (c) 2025 Guillermo Leira Temes
/* */

// this is mine but i dont know if this actually works

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
