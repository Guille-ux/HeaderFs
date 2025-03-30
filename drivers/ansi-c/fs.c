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

// the filesystem is very simple so, write this was simple too

#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//file headers
#define MNUMBER 0xF6F3//magic number, it identifie a sector(s) as a file
#define NS_POS 1// the position on a sector that tells the number of sectors of the file
#define NAME_POS 3 //position where begins the name
#define DATA_BEGIN 27 // begin of the important data
#define DATA_END_SYMBOL 0xF7F2

static File FileTable[NUM_BLOCKS];

#define DISK_BUFF 256 //256 bytes is as sector of uint16_t

uint16_t list_files() {
	uint16_t buffer[DISK_BUFF];
	uint16_t files = 0;
	for (uint32_t i = 0; files < MAX_FILES || i >= MAX_SECTORS; i++) {
		read_block(i, buffer);
		if (buffer[0] == MNUMBER) {
			FileTable[files].ns = buffer[NS_POS];
			FileTable[files].begin = i;
			for (int f = 0; f < 50; f++){
				FileTable[files].name[f] = (uint8_t)buffer[NAME_POS+f];
			}
			FileTable[files].name[51] = '\n';
			i += FileTable[files].ns;
			files++;
		}
	}
	return files;
}

//Header FS funcs, the other is for idiots chatgpt
char write_block(uint32_t block, uint16_t *data) {
	block += PROTECTED_BLOCKS; //protecting blocks
	write_disk(data, block);
	return 0;
}
void read_block(uint32_t block, uint16_t *data) {
        block += PROTECTED_BLOCKS; //protecting blocks
        read_disk(data, block);
}
void write_file(uint16_t sectors, uint16_t *data) {
	uint16_t last_file = list_files() - 1;
	uint32_t pos;
	char win = 1;
	for (int i = 0; i < last_file; i++) {
		if ((FileTable[i].begin + FileTable[i].ns) - (FileTable[i+1].begin + FileTable[i].ns) > sectors) {
			pos = FileTable[i].begin + FileTable[i].ns;
			win = 0;
		}
	}
	if (win) {
		pos = FileTable[last_file].begin + FileTable[last_file].ns;
	}
	for (uint16_t sector = 0; sector < sectors; sector++) {
		uint16_t *buffer = data + 256 * sector;
		write_block(pos+sector, buffer);
	}
}

void add_header(uint16_t *data, uint16_t sectors, const char *name) {
    data[0] = MNUMBER;
    data[NS_POS] = sectors;
    for (int i = 0; i < 50; i++) {
        if (name[i] == '\n') {
            data[NAME_POS + i] = 0;
        } else {
    	    data[NAME_POS + i] = (uint8_t)name[i];
	}
    }

    for (int i = NAME_POS + 50; i < DATA_BEGIN; i++) {
        data[i] = 0;
    }
}
void new_content(uint16_t *data, uint16_t size, const char *name) { // data necesita acabar con '\n'
	int is_begin = 1;
	//data necesita medir un multiplo de 256 y estar la info desplazada 53 bytes
	for (int i = 0; i < size; i++) {
		if (is_begin == 1) {
			is_begin = 0;
			add_header(data[0], size, name);
		}
	}
}
int size_of_file(uint16_t **file) { //size in uitn16_t of a matrix file
	int z = 0;
	for (int i = 0; file[z][i] != DATA_END_SYMBOL; i++) {
		if (i > 256) {
			i = 0;
			z += 1;
		}
	return z*256+i;
	}
}

int size(uint16_t *data) { // midiendo tamaños de una forma normal
	int file_size = 0;
	while (data[file_size] != DATA_END_SYMBOL) {
		file_size++;
	}
	return file_size;
}

void write_new_file(const char *name, uint16_t *data) {
	int data_size = size(data);
	uint16_t tmp[data_size + 256];
	for (int i = 0; i < data_size; i++) {
		tmp[i+256] = data[i];
	}
	int sectors = data_size / 256 + 256;
	add_header(tmp, sectors, name);
	write_file(sectors, *data);
}

uint16_t **read_file(const char *name) {
	int nfile = search_file(name);
	int begin_pos = FileTable[nfile].begin;
	int number_sectors = FileTable[nfile].ns;
	uint16_t file[number_sectors-1][256];
	for (int i = number_sectors-1; i < 256; i++) {
		read_block(begin_pos + i, file[i]);
	}
	return file;
}
int search_file(const char *name) { // returns the number on the File Table
	for (int i = 0; i < MAX_FILES; i++) {
		if (strcmp(FileTable[i].name, name) == 1) {
			return i;
		}
	}
	return -1;
}

uint16_t zeros[256] = {0};

void remove_file(const char *name) { //changes all bits to 0
	int nf = search_file(name);
	File file = FileTable[i];
	uint32_t begin = file.begin; // in this code i'm going to delete de header
	write_block(begin, zeros);
}
//finnish of HeadderFs funcs

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
