/*********************util.html***********************/

/* type.h is ALSO available in samples/type.h  */
#include <stdio.h>
#include <fcntl.h>
#include <linux/fs.h>

//#include <ext2fs/ext2_fs.h>   // NOTE: Ubuntu users MAY NEED "ext2_fs.h"




#include <libgen.h>
#include <string.h>
#include <sys/stat.h>

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs

GD    *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 

#define BLOCK_SIZE     1024

// Block number of EXT2 FS on FD
#define SUPERBLOCK        1
#define GDBLOCK           2
#define ROOT_INODE        2

// Default dir and regulsr file modes
#define DIR_MODE    0040777 
#define FILE_MODE   0100644
#define SUPER_MAGIC  0xEF53
#define SUPER_USER        0

// Proc status
#define FREE              0
#define READY             1
#define RUNNING           2

// Table sizes
#define NMINODES        100
#define NMOUNT           10
#define NPROC            10
#define NFD              10
#define NOFT            100
#define EXT2_NAME_LEN   255

// // Open File Table
// typedef struct oft{
//   int   mode;
//   int   refCount;
//   struct minode *inodeptr;
//   int   offset;
// }OFT;

// // PROC structure
// typedef struct proc{
//   int   uid;
//   int   pid, gid;
//   int   status;
//   struct minode *cwd;
//   OFT   *fd[NFD];
// }PROC;
      
// // In-memory inodes structure
// typedef struct minode{		
//   INODE inode;               // disk inode
//   int   dev, ino;
//   int   refCount;
//   int   dirty;
//   int   mounted;
//   struct mount *mountptr;
// }MINODE;

// // Mount Table structure
// typedef struct mount{
//         int    dev;
//         int    nblocks,ninodes;
//         int    bmap, imap, iblk;
//         MINODE *mounted_inode;
//         char   name[64]; 
//         char   mount_name[64];
// }MOUNT;

/**************************EXT2FD.html***************************/

typedef __u8    u8;           // unsigned char
typedef __u16   u16;          // unsigned short
typedef __u32   u32;          // unsigned long
typedef __u64   u64;

struct ext2_super_block
{
  u32  s_inodes_count;        // total number of inodes
  u32  s_blocks_count;        // total number of blocks
  u32  s_r_blocks_count;     
  u32  s_free_blocks_count;   // current number of free blocks
  u32  s_free_inodes_count;   // current number of free inodes 
  u32  s_first_data_block;    // first data block in this group
  u32  s_log_block_size;      // 0 for 1KB block size
  u32  s_log_frag_size;
  u32  s_blocks_per_group;    // 8192 blocks per group 
  u32  s_frags_per_group;
  u32  s_inodes_per_group;    
  u32  s_mtime;
  u32  s_wtime;
  u16  s_mnt_count;           // number of times mounted 
  u16  s_max_mnt_count;       // mount limit
  u16  s_magic;               // 0xEF53

  u16 s_state;                // File system state 
  u16 s_errors;               // Behaviour when detecting errors 
  u16 s_minor_rev_level;      // minor revision level 
  u32 s_lastcheck;            // time of last check 
  u32 s_checkinterval;        // max. time between checks 
  u32 s_creator_os;           // OS 
  u32 s_rev_level;            // Revision level 
  u16 s_def_resuid;           // Default uid for reserved blocks 
  u16 s_def_resgid;           // Default gid for reserved blocks 
  u32 s_first_ino;            // First non-reserved inode 
  u16     s_inode_size;       //<==========    size of inode structure 

  // some MORE non-essential fileds
};


struct ext2_group_desc
{
  u32  bg_block_bitmap;          // Bmap block number
  u32  bg_inode_bitmap;          // Imap block number
  u32  bg_inode_table;           // Inodes begin block number
  u16  bg_free_blocks_count;     // THESE are OBVIOUS
  u16  bg_free_inodes_count;
  u16  bg_used_dirs_count;        

  u16  bg_pad;                   // ignore these 
  u32  bg_reserved[3];
};

struct ext2_inode 
{
  //*************************************************************************
  u16  i_mode;                // same as st_mode in stat() syscall
  u16  i_uid;                     // ownerID
  u32  i_size;                    // file size in bytes
  u32  i_atime;                   // time fields  
  u32  i_ctime;
  u32  i_mtime;
  u32  i_dtime;
  u16  i_gid;                     // groupID
  u16  i_links_count;             // link count
  u32  i_blocks;                  // IGNORE
  u32  i_flags;                   // IGNORE
  u32  i_reserved1;               // IGNORE
  u32  i_block[15];               // See details below

  /**************************************************************************

    DIRECT blocks : i_block[0] to i_block[11], which point to direct blocks.
    
    INDIRECT block: I_block[12] points to a block, which contians 256 (ulong) 
                    block numbers.
    
    DOUBLE INDIRECT block: 
                    I_block[13] points to a block, which points to 256 blocks,
                    each of which point to 256 blocks.
    
    TRIPLE INDIRECT block: 
                    I_block[14] points to a block, which points to 256 blocks,
                    each of which points to 256 blocks, each of which points to
                    256 blocks.

 *****************************************************************************/                   

  // IGNORE THE FOLLOWING FIELDS
  u32  i_version;
  u32  i_file_acl;
  u32  i_dir_acl;
  u32  i_faddr;
  u8   i_frag;
  u8   i_fsize;
  u16  i_pad1;
  u32  i_reserved2[2];
};

struct ext2_dir_entry_2
{
  u32  inode;        // Inode number; count from 1, NOT from 0
  u16  rec_len;      // This entry length in bytes
  u8   name_len;     // Name length in bytes
  u8   file_type;    // for future use
  char name[EXT2_NAME_LEN];  // File name: 1-255 chars, no NULL byte
};

