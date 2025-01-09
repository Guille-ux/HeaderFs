#ifndef DISK_H
#define DISK_H

#define DISK_FILE "virtual_disk.img"  // disk image
#define DISK_SIZE  1024 * 1024 // size of the disk pls
#define BLOCK_SIZE 512  // size of a block

void disk_init();
void disk_read(unsigned int sector, char* buffer);
void disk_write(unsigned int sector, const char* buffer);

#endif
