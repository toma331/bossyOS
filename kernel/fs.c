#include "headers/fs.h"
#include "headers/screen.h"

#include <stdint.h>
#include <stddef.h>

#define MAX_BLOCKS 128
#define BLOCK_SIZE 1024

#define MEM_OK 0
#define MEM_ERROR 1

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

void *memset(void *dest, int value, size_t count) {
    unsigned char *ptr = dest;
    while (count--) {
        *ptr++ = (unsigned char)value;
    }
    return dest;
}


inode_t inodes[MAX_FILES];
int free_inodes[MAX_FILES];
char blocks[MAX_BLOCKS][BLOCK_SIZE];
dir_block_t *root_dir;


void init_fs() {
    memset(inodes, 0, sizeof(inodes));

    for(int i=0;i<MAX_FILES;i++) free_inodes[i]=1;

    root_dir = (dir_block_t*)blocks[0];
    root_dir->count = 0;

    inodes[0].type = DIR_TYPE;
    inodes[0].size = 0;
    inodes[0].block = 0;
    free_inodes[0] = 0;
}

int allocate_inode(inode_type type) {
    for(int i=0;i<MAX_FILES;i++) {
        if(free_inodes[i]) {
            free_inodes[i] = 0;
            inodes[i].type = type;
            inodes[i].size = 0;
            inodes[i].block = -1;
            return i;
        }
    }
    return -1;
}

int add_dir_entry(dir_block_t *dir, const char *name, int inode_num) {
    if(dir->count >= MAX_FILES) return -1;
    strncpy(dir->entries[dir->count].name, name, MAX_NAME - 1);

    dir->entries[dir->count].name[MAX_NAME - 1] = '\0';
    dir->entries[dir->count].inode_number = inode_num;
    dir->count++;
    return 0;
}

void mkdir_fs(const char *name) {
    int inode_num = allocate_inode(DIR_TYPE);
    if(inode_num < 0) { print("Нет свободных inode\n"); return; }
    int block_num = inode_num; 
    inodes[inode_num].block = block_num;
    dir_block_t *dir = (dir_block_t*)blocks[block_num];
    dir->count = 0;
    put_char('\n');
    add_dir_entry(root_dir, name, inode_num);
}

void touch_fs(const char *name) {
    int inode_num = allocate_inode(FILE_TYPE);
    if(inode_num < 0) { print("Нет свободных inode\n"); return; }
    int block_num = inode_num; 
    inodes[inode_num].block = block_num;
    put_char('\n');
    add_dir_entry(root_dir, name, inode_num);
}

void ls_fs() {
    for(int i=0;i<root_dir->count;i++) {
        int inode_num = root_dir->entries[i].inode_number;

        char *type = inodes[inode_num].type == DIR_TYPE ? "DIR" : "FILE";

        print(root_dir->entries[i].name);
        print(type);
        put_char('\n');
    }
}
