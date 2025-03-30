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

#ifndef DISK_H
#define DISK_H

#define DISK_FILE "virtual_disk.img"  // disk image
#define DISK_SIZE  1024 * 1024 // size of the disk pls
#define BLOCK_SIZE 512  // size of a block

void disk_init();
void disk_read(unsigned int sector, char* buffer);
void disk_write(unsigned int sector, const char* buffer);

#endif
