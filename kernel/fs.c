#include "./headers/screen.h"

#include <stdint.h>
#include <stddef.h>

#define BLOCK_SIZE 1024
#define MAX_FILES 128
#define MAX_NAME 32
#define MAX_BLOCKS 128

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

typedef enum {
    FILE_TYPE,
    DIR_TYPE
} inode_type;

typedef struct {
    inode_type type;
    int size;
    int block; // индекс блока данных
} inode_t;

typedef struct {
    char name[MAX_NAME];
    int inode_number;
} dir_entry_t;

typedef struct {
    dir_entry_t entries[MAX_FILES];
    int count;
} dir_block_t;


// Файловая система в памяти
inode_t inodes[MAX_FILES];
int free_inodes[MAX_FILES];
char blocks[MAX_BLOCKS][BLOCK_SIZE];
dir_block_t *root_dir;


// Инициализация FS
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

// Выделение нового inode
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

// Добавление файла или папки в директорию
int add_dir_entry(dir_block_t *dir, const char *name, int inode_num) {
    if(dir->count >= MAX_FILES) return -1;
    strncpy(dir->entries[dir->count].name, name, MAX_NAME - 1);

    dir->entries[dir->count].name[MAX_NAME - 1] = '\0';
    dir->entries[dir->count].inode_number = inode_num;
    dir->count++;
    return 0;
}

// mkdir
void mkdir_fs(const char *name) {
    int inode_num = allocate_inode(DIR_TYPE);
    if(inode_num < 0) { print("Нет свободных inode\n"); return; }
    int block_num = inode_num; // для простоты
    inodes[inode_num].block = block_num;
    dir_block_t *dir = (dir_block_t*)blocks[block_num];
    dir->count = 0;
    put_char('\n');
    add_dir_entry(root_dir, name, inode_num);
}

// touch
void touch_fs(const char *name) {
    int inode_num = allocate_inode(FILE_TYPE);
    if(inode_num < 0) { print("Нет свободных inode\n"); return; }
    int block_num = inode_num; // для простоты
    inodes[inode_num].block = block_num;
    put_char('\n');
    add_dir_entry(root_dir, name, inode_num);
}

// ls
void ls_fs() {
    for(int i=0;i<root_dir->count;i++) {
        int inode_num = root_dir->entries[i].inode_number;

        char *type = inodes[inode_num].type == DIR_TYPE ? "DIR" : "FILE";

        print(root_dir->entries[i].name);
        print(type);
        put_char('\n');
    }
}
