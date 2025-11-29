#ifndef FS
#define FS

#define MAX_FILES 128
#define MAX_NAME 32

typedef enum
{
	FILE_TYPE,
	DIR_TYPE
} inode_type;

typedef struct
{
	inode_type type;
	int size;
	int block;
} inode_t;

typedef struct
{
	char name[MAX_NAME];
	int inode_number;
} dir_entry_t;

typedef struct
{
	dir_entry_t entries[MAX_FILES];
	int count;
} dir_block_t;

void init_fs();
void mkdir_fs(const char *name);
void touch_fs(const char *name);
void ls_fs();

#endif
