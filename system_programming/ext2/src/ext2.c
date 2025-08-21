/**
	Written By: Tom Golzman
	Date: 17/08/2025
	Reviewed By: 
**/

/************************************ Includes *************************************/
#include <assert.h> 		/* assert() */
#include <linux/types.h>	/* __u8, __u16, __u32, __s16, __s32*/

#include "utils.h"	/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM(), ExitIfBad() */

#include "ext2.h"

/************************************* Enums *************************************/
enum
{
	NUM_BLOCKS = 12,
	SUPER_BLOCK_MAGIC_NUM = 0xEF53,
	BASE_OFFSET = 1024,
	ROOT_INODE_NUM = 2,
	ASCII_SIZE = 256
};

/************************************* Structs *************************************/
typedef struct ext2_super_block {
	__u32	s_inodes_count;		/* Inodes count */
	__u32	s_blocks_count;		/* Blocks count */
	__u32	s_r_blocks_count;	/* Reserved blocks count */
	__u32	s_free_blocks_count;/* Free blocks count */
	__u32	s_free_inodes_count;/* Free inodes count */
	__u32	s_first_data_block;	/* First Data Block */
	__u32	s_log_block_size;	/* Block size */
	__s32	s_log_frag_size;	/* Fragment size */
	__u32	s_blocks_per_group;	/* # Blocks per group */
	__u32	s_frags_per_group;	/* # Fragments per group */
	__u32	s_inodes_per_group;	/* # Inodes per group */
	__u32	s_mtime;			/* Mount time */
	__u32	s_wtime;			/* Write time */
	__u16	s_mnt_count;		/* Mount count */
	__s16	s_max_mnt_count;	/* Maximal mount count */
	__u16	s_magic;			/* Magic signature */
	__u16	s_state;			/* File system state */
	__u16	s_errors;			/* Behaviour when detecting errors */
	__u16	s_minor_rev_level; 	/* minor revision level */
	__u32	s_lastcheck;		/* time of last check */
	__u32	s_checkinterval;	/* max. time between checks */
	__u32	s_creator_os;		/* OS */
	__u32	s_rev_level;		/* Revision level */
	__u16	s_def_resuid;		/* Default uid for reserved blocks */
	__u16	s_def_resgid;		/* Default gid for reserved blocks */
	__u32	s_first_ino; 		/* First non-reserved inode */
	__u16   s_inode_size; 		/* size of inode structure */
	__u16	s_block_group_nr; 	/* block group # of this super_block */
	__u32	s_feature_compat; 	/* compatible feature set */
	__u32	s_feature_incompat; /* incompatible feature set */
	__u32	s_feature_ro_compat;/* readonly-compatible feature set */
	__u8	s_uuid[16];			/* 128-bit uuid for volume */
	char	s_volume_name[16]; 	/* volume name */
	char	s_last_mounted[64];	/* directory where last mounted */
	__u32	s_algorithm_usage_bitmap; /* For compression */
	__u8	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	__u16	s_padding1;
	__u32	s_reserved[204];	/* Padding to the end of the block */
} ext2_super_block_t;

typedef struct ext2_group_desc
{
	__u32	bg_block_bitmap;		/* Blocks bitmap block */
	__u32	bg_inode_bitmap;		/* Inodes bitmap block */
	__u32	bg_inode_table;			/* Inodes table block */
	__u16	bg_free_blocks_count;	/* Free blocks count */
	__u16	bg_free_inodes_count;	/* Free inodes count */
	__u16	bg_used_dirs_count;		/* Directories count */
	__u16	bg_pad;					/* Padding */
	__u32	bg_reserved[3];
} ext2_group_desc_t;

typedef struct ext2_inode {
	__u16	i_mode;			/* File mode */
	__u16	i_uid;			/* Low 16 bits of Owner Uid */
	__u32	i_size;			/* Size in bytes */
	__u32	i_atime;		/* Access time */
	__u32	i_ctime;		/* Creation time */
	__u32	i_mtime;		/* Modification time */
	__u32	i_dtime;		/* Deletion Time */
	__u16	i_gid;			/* Low 16 bits of Group Id */
	__u16	i_links_count;	/* Links count */
	__u32	i_blocks;		/* Blocks count */
	__u32	i_flags;		/* File flags */
	union {
		struct {
			__u32  l_i_reserved1;
		} linux1;
		struct {
			__u32  h_i_translator;
		} hurd1;
		struct {
			__u32  m_i_reserved1;
		} masix1;
	} osd1;	/* OS dependent 1 */
	__u32	i_block[NUM_BLOCKS];/* Pointers to blocks */
	__u32	i_generation;		/* File version (for NFS) */
	__u32	i_file_acl;			/* File ACL */
	__u32	i_dir_acl;			/* Directory ACL */
	__u32	i_faddr;			/* Fragment address */
	union {
		struct {
			__u8	l_i_frag;		/* Fragment number */
			__u8	l_i_fsize;		/* Fragment size */
			__u16	i_pad1;
			__u16	l_i_uid_high;	/* these 2 fields    */
			__u16	l_i_gid_high;	/* were reserved2[0] */
			__u32	l_i_reserved2;
		} linux2;
		struct {
			__u8	h_i_frag;		/* Fragment number */
			__u8	h_i_fsize;		/* Fragment size */
			__u16	h_i_mode_high;
			__u16	h_i_uid_high;
			__u16	h_i_gid_high;
			__u32	h_i_author;
		} hurd2;
		struct {
			__u8	m_i_frag;		/* Fragment number */
			__u8	m_i_fsize;		/* Fragment size */
			__u16	m_pad1;
			__u32	m_i_reserved2[2];
		} masix2;
	} osd2;	/* OS dependent 2 */
} ext2_inode_t;

typedef struct ext2_dir_entry {
	__u32	inode;		/* Inode number */
	__u16	rec_len;	/* Directory entry length */
	__u16	name_len;	/* Name length */
} ext2_dir_entry_t;

/******************************** Static Functions *********************************/
static FILE* OpenDevice(const char* device_path_);
static int ReadSuperBlock(FILE* device_, ext2_super_block_t* sb_out_);
static int ReadGroupDescriptor(FILE* device_, ext2_super_block_t* sb_out_, ext2_group_desc_t* gd_out_);
static int IsFileInRoot(FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, const char* file_name_, ext2_inode_t* out_inode_);
static int IsFileInDirectory(FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, const ext2_inode_t* dir_inode_, const char* file_name_, ext2_inode_t* out_inode_);
static int IsFileNameInBlock(char* block_, size_t block_size_, const char* file_name_, FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, ext2_inode_t* out_inode_);
static int IsEntryNameMatch(ext2_dir_entry_t* entry_, const char* name_);
static int ReadInode(FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, unsigned int inode_num_, ext2_inode_t* out_inode_);
static int PrintInodeContent(FILE* device_, const ext2_super_block_t* sb, const ext2_inode_t* inode_);

/************************************ Functions ************************************/
int EXT2PrintSuperblock(const char* device_path_)
{
	FILE* device = NULL;
	unsigned int block_size = 0;
	ext2_super_block_t super_block = {0};
	int status = 0;

	/* assert */
	assert(NULL != device_path_);

	/* open device */
    device = OpenDevice(device_path_);
	RET_IF_BAD(NULL != device, FAIL, "EXT2PrintSuperblock(): OpenDevice() FAILED!\n");

	/* read super block */
	status = ReadSuperBlock(device, &super_block);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "EXT2PrintSuperblock(): ReadSuperBlock() FAILED!\n", fclose(device));
	
	/* check if it is an ext2 file system*/
	RET_IF_BAD_CLEAN(super_block.s_magic == SUPER_BLOCK_MAGIC_NUM, FAIL, "not an ext2 file system!\n", fclose(device));

	/* close the device */
	status = fclose(device);
	RET_IF_BAD(status == 0, FAIL, "Ex2PrintFileContent(): fclose() FAILED!\n");

	block_size = BASE_OFFSET << super_block.s_log_block_size;

	printf("Reading super_block-block from device %s:\n"
	       "Inodes count            : %u\n"
	       "Blocks count            : %u\n"
	       "Reserved blocks count   : %u\n"
	       "Free blocks count       : %u\n"
	       "Free inodes count       : %u\n"
	       "First data block        : %u\n"
	       "Block size              : %u\n"
	       "Blocks per group        : %u\n"
	       "Inodes per group        : %u\n"
	       "Creator OS              : %u\n"
	       "First non-reserved inode: %u\n"
	       "Size of inode structure : %hu\n"
	       ,
		   device_path_,
	       super_block.s_inodes_count,
	       super_block.s_blocks_count,
	       super_block.s_r_blocks_count,
	       super_block.s_free_blocks_count,
	       super_block.s_free_inodes_count,
	       super_block.s_first_data_block,
	       block_size,
	       super_block.s_blocks_per_group,
	       super_block.s_inodes_per_group,
	       super_block.s_creator_os,
	       super_block.s_first_ino,
	       super_block.s_inode_size
	);

	return SUCCESS;
}

int EXT2PrintGroupDesc(const char* device_path_)
{
	FILE* device = NULL;
	ext2_super_block_t super_block = {0};
	ext2_group_desc_t group_desc = {0};
	int status = 0;

	/* assert */
	assert(NULL != device_path_);

	/* open device */
    device = OpenDevice(device_path_);
	RET_IF_BAD(NULL != device, FAIL, "EXT2PrintGroupDesc(): OpenDevice() FAILED!\n");
	
	/* read super block */
	status = ReadSuperBlock(device, &super_block);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "EXT2PrintGroupDesc(): ReadSuperBlock() FAILED!\n", fclose(device));

	/* check if it is an ext2 file system*/
	RET_IF_BAD_CLEAN(super_block.s_magic == SUPER_BLOCK_MAGIC_NUM, FAIL, "not an ext2 file system!\n", fclose(device));

	/* read group descriptor */
	status = ReadGroupDescriptor(device, &super_block, &group_desc);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "EXT2PrintGroupDesc(): ReadGroupDescriptor() FAILED!\n", fclose(device));

	/* close the device */
	status = fclose(device);
	RET_IF_BAD(status == 0, FAIL, "Ex2PrintFileContent(): fclose() FAILED!\n");
	
	printf("Reading first group-descriptor from device %s:\n"
	       "Blocks bitmap block: %u\n"
	       "Inodes bitmap block: %u\n"
	       "Inodes table block : %u\n"
	       "Free blocks count  : %u\n"
	       "Free inodes count  : %u\n"
	       "Directories count  : %u\n"
	       ,
		   device_path_,
	       group_desc.bg_block_bitmap,
	       group_desc.bg_inode_bitmap,
	       group_desc.bg_inode_table,
	       group_desc.bg_free_blocks_count,
	       group_desc.bg_free_inodes_count,
	       group_desc.bg_used_dirs_count
	);
	
	return SUCCESS;
}

int Ex2PrintFileContentInRoot(const char* device_path_, const char* file_name_)
{
	FILE* device = NULL;
	ext2_super_block_t super_block = {0};
	ext2_group_desc_t group_desc = {0};
    ext2_inode_t file_inode = {0};
	int status = 0;

	/* assert */
	assert(NULL != device_path_);
	assert(NULL != file_name_);

	/* open device */
	device = OpenDevice(device_path_);
	RET_IF_BAD(NULL != device, FAIL, "Ex2PrintFileContentInRoot(): OpenDevice() FAILED!\n");

	/* read super block */
	status = ReadSuperBlock(device, &super_block);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "Ex2PrintFileContentInRoot(): ReadSuperBlock() FAILED!\n", fclose(device));
	
	/* read group descriptor */
	status = ReadGroupDescriptor(device, &super_block, &group_desc);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "Ex2PrintFileContentInRoot(): ReadGroupDescriptor() FAILED!\n", fclose(device));

	/* find inode by file path */
	status = IsFileInRoot(device, &super_block, &group_desc, file_name_, &file_inode);
	RET_IF_BAD_CLEAN(TRUE == status, FAIL, "Ex2PrintFileContentInRoot(): file is not in root directory!\n", fclose(device));

	/* print file content */
	status = PrintInodeContent(device, &super_block, &file_inode);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "Ex2PrintFileContentInRoot(): PrintInodeContent() FAILED!\n", fclose(device));

	/* close device */
	status = fclose(device);
	RET_IF_BAD(status == 0, FAIL, "Ex2PrintFileContentInRoot(): fclose() FAILED!\n");

	return SUCCESS;
}

int Ex2PrintFileContent(const char* device_path_, const char* file_path_)
{
	FILE* device = NULL;
	ext2_super_block_t super_block = {0};
	ext2_group_desc_t group_desc = {0};
	ext2_inode_t curr_dir_inode = {0};
    ext2_inode_t file_inode = {0};
	const char* remaining_path = (assert(NULL != file_path_), file_path_);
	char next_file_name[ASCII_SIZE];
	int status = 0;

	/* assert */
	assert(NULL != device_path_);
	assert('/' == file_path_[0]);

	/* open device */
	device = OpenDevice(device_path_);
	RET_IF_BAD(NULL != device, FAIL, "Ex2PrintFileContent(): OpenDevice() FAILED!\n");

	/* read super block */
	status = ReadSuperBlock(device, &super_block);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "Ex2PrintFileContent(): ReadSuperBlock() FAILED!\n", fclose(device));
	
	/* read group descriptor */
	status = ReadGroupDescriptor(device, &super_block, &group_desc);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "Ex2PrintFileContent(): ReadGroupDescriptor() FAILED!\n", fclose(device));

	/* start read inodes from the root */
	status = ReadInode(device_, &super_block, &group_desc, ROOT_INODE_NUM, &curr_dir_inode);
	RET_IF_BAD_CLEAN(SUCCESS == status, FAIL, "Ex2PrintFileContent(): ReadInode() FAILED!\n", fclose(device));

	/* traverse path files */
	while (1)
	{
		remaining_path = NextPathToken(remaining_path, next_file_name);

		/* check if there are no more tokens */
		RET_IF_BAD_CLEAN(next_file_name[0] != '\0', FAIL, "Ex2PrintFileContent(): no more tokens!\n", fclose(device));

		/* find the file in the current directory */
		status = IsFileInDirectory(device, &super_block, &group_desc, &curr_dir_inode, next_file_name, &file_inode);
		RET_IF_BAD_CLEAN(TRUE == status, FAIL, "Ex2PrintFileContent(): file not found!\n", fclose(device));

	}
}

/******************************** Static Functions *********************************/
static FILE* OpenDevice(const char* device_path_)
{
	FILE* device = NULL;

	/* assert */
	assert(NULL != device_path_);

	device = fopen(device_path_, "r");
	RET_IF_BAD(NULL != device, NULL, "OpenDevice(): fopen() FAILED!\n");

	return device;
}

static int ReadSuperBlock(FILE* device_, ext2_super_block_t* sb_out_)
{
	int status = 0;

	/* assert */
	assert(NULL != device_);
	assert(NULL != sb_out_);

	/* seek to super block */
	status = fseek(device_, BASE_OFFSET, SEEK_SET);
	RET_IF_BAD(status == 0, FAIL, "EXT2PrintSuperblock(): fseek() FAILED!\n");

	/* read super block */
	status = fread(sb_out_, sizeof(ext2_super_block_t), 1, device_);
	RET_IF_BAD(status == 1, FAIL, "EXT2PrintSuperblock(): fread() FAILED!\n");

	return SUCCESS;
}

static int ReadGroupDescriptor(FILE* device_, ext2_super_block_t* sb_, ext2_group_desc_t* gd_out_)
{
	unsigned int block_size = 0;
	int status = 0;

	/* assert */
	assert(NULL != device_);
	assert(NULL != sb_);
	assert(NULL != gd_out_);

	/* read super block to claculate block size */
	status = ReadSuperBlock(device_, sb_);
	RET_IF_BAD(SUCCESS == status, FAIL, "ReadGroupDescriptor(): ReadSuperBlock() FAILED!\n");

	/* calculate block size */
	block_size = BASE_OFFSET << sb_->s_log_block_size;
	
	/* seek to group descriptor */
	status = fseek(device_, block_size, SEEK_SET);
	RET_IF_BAD(status == 0, FAIL, "ReadGroupDescriptor(): fseek() FAILED!\n");

	/* read group descriptor */
	status = fread(gd_out_, sizeof(ext2_group_desc_t), 1, device_);
	RET_IF_BAD(status == 1, FAIL, "ReadGroupDescriptor(): fread() FAILED!\n");

	return SUCCESS;
}

static int IsFileInRoot(FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, const char* file_name_, ext2_inode_t* out_inode_)
{
	ext2_inode_t root_inode = {0};
	int status = 0;

	/* assert */
	assert(NULL != device_);
	assert(NULL != sb_);
	assert(NULL != gd_);
	assert(NULL != out_inode_);

	/* read inode */
	status = ReadInode(device_, sb_, gd_, ROOT_INODE_NUM, &root_inode);
	RET_IF_BAD(SUCCESS == status, FALSE, "FindFileInRoot(): ReadInode() FAILED!\n");

	/* return if file is in the root directory */
	return IsFileInDirectory(device_, sb_, gd_, &root_inode, file_name_, out_inode_);
}

static int IsFileInDirectory(FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, const ext2_inode_t* dir_inode_, const char* file_name_, ext2_inode_t* out_inode_)
{
	unsigned int block_size = (assert(NULL != sb_), BASE_OFFSET << sb_->s_log_block_size);
	char* curr_block = NULL;
	int status = 0;
	int i = 0;

	/* assert */
	assert(NULL != device_);
	assert(NULL != gd_);
	assert(NULL != dir_inode_);
	assert(NULL != out_inode_);

	curr_block = (char*)calloc(1, block_size);
	RET_IF_BAD(NULL != curr_block, FALSE, "IsFileInDirectory(): calloc() FAILED!\n");

	/* for each data block of the directory inode */
	for (i = 0; i < NUM_BLOCKS && dir_inode_->i_block[i] > 0; ++i)
	{
		/* seek to the i-th block */
		status = fseek(device_, dir_inode_->i_block[i] * block_size, SEEK_SET);
		RET_IF_BAD(status == 0, FALSE, "IsFileInDirectory(): fseek() FAILED!\n");

		/* read the block */
		status = fread(curr_block, block_size, 1, device_);
		RET_IF_BAD(status == 1, FALSE, "IsFileInDirectory(): fread() FAILED!\n");

		/* check if the file name is in the current block */
		if (IsFileNameInBlock(curr_block, block_size, file_name_, device_, sb_, gd_, out_inode_))
		{
			/* free curr_block */
			free(curr_block);
			curr_block = NULL;

			/* return TRUE */
			return TRUE;
		}
	}

	/* free curr_block */
	free(curr_block);
	curr_block = NULL;

	/* return FALSE */
	return FALSE;
}

static int IsFileNameInBlock(char* block_, size_t block_size_, const char* file_name_, FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, ext2_inode_t* out_inode_)
{
	ext2_dir_entry_t* curr_entry = (assert(NULL != block_), (ext2_dir_entry_t*)block_);
	int status = 0;

	/* assert */
	assert(NULL != file_name_);
	assert(NULL != device_);
	assert(NULL != sb_);
	assert(NULL != gd_);
	assert(NULL != out_inode_);

	/* while not reached the end of the block */
	while ((char*)curr_entry < block_ + block_size_)
	{
		/* check if current entry's name is the file name */
		if (IsEntryNameMatch(curr_entry, file_name_))
		{
			/* read the current inode from the disk to out_inode */
			status = ReadInode(device_, sb_, gd_, curr_entry->inode, out_inode_);
			RET_IF_BAD(SUCCESS == status, FALSE, "IsFileNameInBlock(): ReadInode FAILED!\n");

			return TRUE;
		}

		/* move to the next directory entry in the block */
		curr_entry = (ext2_dir_entry_t*)((char*)curr_entry + curr_entry->rec_len);
	}

	return FALSE;
}

static int IsEntryNameMatch(ext2_dir_entry_t* entry_, const char* file_name_)
{
	int cmp_result = 0;
	char* entry_name = (char*)calloc(entry_->name_len + 1, 1);
	RET_IF_BAD(NULL != entry_name, FALSE, "IsEntryNameMatch(): calloc FAILED!\n");

	/* assert */
	assert(NULL != entry_);
	assert(NULL != file_name_);

	/* copy the entry name into local variable of entry name*/
	memcpy(entry_name, (char*)entry_ + sizeof(ext2_dir_entry_t), entry_->name_len);

	/* add null terminator at the end of the entry name */
	entry_name[entry_->name_len] = '\0';

	/* compare the entry name with the file name */
	cmp_result = strcmp(entry_name, file_name_);

	/* free the entry_name */
	free(entry_name);
	entry_name = NULL;

	/* return compare result */
	return (cmp_result == 0) ? TRUE : FALSE;
}

static int ReadInode(FILE* device_, const ext2_super_block_t* sb_, const ext2_group_desc_t* gd_, unsigned int inode_num_, ext2_inode_t* out_inode_)
{
	unsigned int block_size = (assert(NULL != sb_), BASE_OFFSET << sb_->s_log_block_size);
	unsigned int inode_table_block = (assert(NULL != gd_), gd_->bg_inode_table);
	unsigned int index = inode_num_ - 1;
	unsigned int inode_size = sb_->s_inode_size;
	unsigned int offset = (inode_table_block * block_size) + (index * inode_size);
	int status = 0;

	/* assert */
	assert(NULL != device_);
	assert(NULL != out_inode_);

	/* seek to inode location in the inode table */
	status = fseek(device_, offset, SEEK_SET);
	RET_IF_BAD(status == 0, FAIL, "ReadInode(): fseek() FAILED!\n");

	/* read inode structure from the disk */
	status = fread(out_inode_, sizeof(ext2_inode_t), 1, device_);
	RET_IF_BAD(status == 1, FAIL, "IsFileInDirectory(): fread() FAILED!\n");

	return SUCCESS;
}

static int PrintInodeContent(FILE* device_, const ext2_super_block_t* sb_, const ext2_inode_t* inode_)
{
	unsigned int block_size = (assert(NULL != sb_), BASE_OFFSET << sb_->s_log_block_size);
	char* curr_block = NULL;
	size_t total_bytes_left_to_write = inode_->i_size;
	size_t to_read = 0;
	size_t fread_result = 0;
	size_t fwrite_result = 0;
	int status = 0;
	int i = 0;

	/* assert */
	assert(NULL != device_);
	assert(NULL != inode_);

	curr_block = (char*)calloc(1, block_size);
	RET_IF_BAD(NULL != curr_block, FAIL, "PrintInodeContent(): calloc() FAILED!\n");

	/* for each block */
	for (i = 0; i < NUM_BLOCKS && inode_->i_block[i] > 0 && total_bytes_left_to_write > 0; ++i)
	{
		/* seek to the start of the block */
		status = fseek(device_, inode_->i_block[i] * block_size, SEEK_SET);
		RET_IF_BAD(status == 0, FAIL, "PrintInodeContent(): fseek() FAILED!\n");

		/* calculate how many bytes need to read */
		to_read = (total_bytes_left_to_write < block_size) ? total_bytes_left_to_write : block_size;

		/* read the content of the current block */
		fread_result = fread(curr_block, 1, to_read, device_);
		RET_IF_BAD(fread_result == to_read, FAIL, "PrintInodeContent(): fread() FAILED!\n");

		/* print the block content (write it to stdout) */
		fwrite_result = fwrite(curr_block, 1, to_read, stdout);
		RET_IF_BAD(fwrite_result == to_read, FAIL, "PrintInodeContent(): fwrite() FAILED!\n");

		total_bytes_left_to_write -= to_read;
	}

	/* free curr_block */
	free(curr_block);
	curr_block = NULL;

	return SUCCESS;
}