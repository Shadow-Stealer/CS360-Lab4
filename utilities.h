

#include "type.h"
#include <math.h>

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLOCK_SIZE, 0);
  read(fd, buf, BLOCK_SIZE);
}

int put_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLOCK_SIZE, 0);
  write(fd, buf, BLOCK_SIZE);
}

int nGroups(int blocksPerGroup, int nBlocks)
{
  if(nBlocks % blocksPerGroup == 0)
  {
    return nBlocks / blocksPerGroup;
  }
  else
  {
    return (nBlocks / blocksPerGroup) + 1;
  }
}

super(int fd)
{
  char* buf[BLOCK_SIZE];
  // read SUPER block
  get_block(fd, 1, buf);  
  sp = (SUPER *)buf;

  printf("********************BEGIN SUPER BLOCK********************\n");
  // check for EXT2 magic number:

  printf("s_magic = %x\n", sp->s_magic);
  if (sp->s_magic != 0xEF53){
    printf("NOT an EXT2 FS\n");
    exit(1);
  }

  

  printf("EXT2 FS OK\n");



  printf("s_inodes_count = %d\n", sp->s_inodes_count);
  printf("s_blocks_count = %d\n", sp->s_blocks_count);

  printf("s_free_inodes_count = %d\n", sp->s_free_inodes_count);
  printf("s_free_blocks_count = %d\n", sp->s_free_blocks_count);
  printf("s_first_data_blcok = %d\n", sp->s_first_data_block);


  printf("s_log_block_size = %d\n", sp->s_log_block_size);
  printf("s_log_frag_size = %d\n", sp->s_log_frag_size);

  printf("s_blocks_per_group = %d\n", sp->s_blocks_per_group);

  printf("ngroups = %d\n", nGroups(sp->s_blocks_per_group, sp->s_blocks_count));


  printf("s_frags_per_group = %d\n", sp->s_frags_per_group);
  printf("s_inodes_per_group = %d\n", sp->s_inodes_per_group);


  printf("s_mnt_count = %d\n", sp->s_mnt_count);
  printf("s_max_mnt_count = %d\n", sp->s_max_mnt_count);

  printf("s_magic = %x\n", sp->s_magic);

  printf("s_mtime = %s", ctime(&sp->s_mtime));
  printf("s_wtime = %s", ctime(&sp->s_wtime));

  printf("********************END SUPER BLOCK********************\n");

}
/**********************END OF SUPER****************************/

/**********************groupDescriptor*************************/
groupDescriptor(int fd)
{
  char buf[BLOCK_SIZE];
  get_block(fd, 2, buf);

  gp = (GD*)buf;

  printf("********************BEGIN GROUP GROUP DESCRIPTOR BLOCK********************\n");

  printf("Bmap Block Number = %d\n", gp->bg_block_bitmap);
  printf("Imap Block Number = %d\n", gp->bg_inode_bitmap);
  printf("Inodes begin block number = %d\n", gp->bg_inode_table);
  printf("free blocks = %d\n", gp->bg_free_blocks_count);
  printf("free inodes = %d\n", gp->bg_free_inodes_count);
  printf("Used Directories count = %d\n", gp->bg_used_dirs_count);
  printf("********************END GROUP GROUP DESCRIPTOR BLOCK********************\n");

}
/**********************END OF groupDescriptor*******************/


/**********************tst_bit*******************/
int tst_bit(char *buf, int bit)
{
  int i, j;
  i = bit / 8;  j = bit % 8;
  if (buf[i] & (1 << j))
  {
     return 1;
  }
  return 0;
}
/**********************END OF tst_bit*******************/

int set_bit(char* buf, int bit)
{
  int i, j;
  i = bit / 8;
  j = bit % 8;
  buf[i] |= (1 << j);
}

int clr_bit(char* buf, int bit)
{
  int i, j;
  i = bit / 8;
  j = bit % 8;

  buf[i] &= ~(1 << j);
}

int decFreeInodes(int fd)
{
  int i;
  char buf[BLOCK_SIZE];

  //decrement free inodes count in SUPER and GD
  get_block(fd, 1, buf);
  sp = (SUPER*)buf;
  sp->s_free_inodes_count--;
  put_block(fd, 1, buf);

  get_block(fd, 2, buf);
  gp = (GD*)buf;
  gp->bg_free_inodes_count--;
  put_block(fd, 2, buf);  
}

int incFreeInodes(int fd)
{
  int i;
  char buf[BLOCK_SIZE];
  get_block(fd, 1, buf);
  sp = (SUPER*)buf;
  sp->s_free_inodes_count++;
  put_block(fd, 1, buf);

  get_block(fd, 2, buf);
  gp = (GD*)buf;
  gp->bg_free_inodes_count++;
  put_block(fd, 2, buf); 
}

int decFreeBlocks(int fd)
{
  int i;
  char buf[BLOCK_SIZE];

  //decrement free inodes count in SUPER and GD
  get_block(fd, 1, buf);
  sp = (SUPER*)buf;
  sp->s_free_blocks_count--;
  put_block(fd, 1, buf);

  get_block(fd, 2, buf);
  gp = (GD*)buf;
  gp->bg_free_blocks_count--;
  put_block(fd, 2, buf);  
}

int incFreeBlocks(int fd)
{
  int i;
  char buf[BLOCK_SIZE];

  //decrement free inodes count in SUPER and GD
  get_block(fd, 1, buf);
  sp = (SUPER*)buf;
  sp->s_free_blocks_count++;
  put_block(fd, 1, buf);

  get_block(fd, 2, buf);
  gp = (GD*)buf;
  gp->bg_free_blocks_count++;
  put_block(fd, 2, buf);  
}



int getImap(int fd)
{
  char buf[BLOCK_SIZE];
  
  get_block(fd, 2, buf);

  gp = (GD*)buf;
  
  return gp->bg_inode_bitmap;  
}

int getBmap(int fd)
{
  char buf[BLOCK_SIZE];

  get_block(fd, 2, buf);

  gp = (GD*)buf;

  return gp->bg_block_bitmap;
}

int getIBlock(int fd)
{
  char buf[BLOCK_SIZE];
  
  get_block(fd, 2, buf);
  gp = (GD*)buf;

  return gp->bg_inode_table;
}


int getNInodes(int fd)
{
  
  char buf[BLOCK_SIZE];

  get_block(fd, 1, buf);

  sp = (SUPER*)buf;

  return sp->s_inodes_count;
}

int getNBlocks(int fd)
{
  char buf[BLOCK_SIZE];

  get_block(fd, 1, buf);

  sp = (SUPER*)buf;

  return sp->s_blocks_count;
}

/**********************ialloc*******************/
int ialloc(int fd)
{
  int i;
  char buf[BLOCK_SIZE];
  int imap;
  int ninodes;
  
  ninodes = getNInodes(fd);  
  imap = getImap(fd);

  get_block(fd, imap, buf);

  for(i = 0; i < ninodes; i++)
  {
    if(tst_bit(buf, i) == 0)
    {
      set_bit(buf, i);
      decFreeInodes(fd);

      put_block(fd, imap, buf);

      return i+1;
    }
  }

  printf("ialloc(): no more free inodes\n");
  return 0;
}
/**********************END ialloc*******************/

/**********************iDEalloc*******************/
int iDEalloc(int fd, int inodeNumber)
{
  int i;
  char buf[BLOCK_SIZE];
  int imap;
  int ninodes;

  ninodes = getNInodes(fd);
  imap = getImap(fd);

  if(inodeNumber <= ninodes)
  {
    //inode is in range
    get_block(fd, imap, buf);
  
    if(tst_bit(buf, inodeNumber) == 0)
    {
      printf("iDEalloc(): inode is not allocated, %d\n", inodeNumber);
      return -1;
    }
    else
    {
      //inode is allocated need to deallocate it
      clr_bit(buf, inodeNumber);
      incFreeInodes(fd);
      put_block(fd, imap, buf);

      return 0;
    }
    
  }
  else
  {
    printf("iDEalloc(): inodeNumber is out of range %d\n", inodeNumber);
    return -1;
  }
}
/**********************END iDEalloc*******************/

/**********************bDEalloc*******************/
int bDEalloc(int fd, int BlockNumber)
{
  int i;
  char buf[BLOCK_SIZE];
  int bmap;
  int nblocks;

  nblocks = getNBlocks(fd);
  bmap = getBmap(fd);

  if(BlockNumber <= nblocks)
  {
    //inode is in range
    get_block(fd, bmap, buf);
  
    if(tst_bit(buf, BlockNumber) == 0)
    {
      printf("bDEalloc(): block is not allocated, %d\n", BlockNumber);
      return -1;
    }
    else
    {
      //block is allocated need to deallocate it
      clr_bit(buf, BlockNumber);
      incFreeInodes(fd);
      put_block(fd, bmap, buf);

      return 0;
    }
    
  }
  else
  {
    printf("bDEalloc(): BlockNumber is out of range %d\n", BlockNumber);
    return -1;
  }
}
/**********************END iDEalloc*******************/



/**********************balloc*******************/
int balloc(int fd)
{
  int i;
  char buf[BLOCK_SIZE];
  int bmap;
  int nbnodes;

  bmap = getBmap(fd);
  nbnodes = getNBlocks(fd);

  get_block(fd, bmap, buf);

  for(i = 0; i < nbnodes; i++)
  {
    if(tst_bit(buf, i) == 0)
    {
      set_bit(buf, i);
      decFreeBlocks(fd);

      put_block(fd, bmap, buf);

      return i+1;
    }

    printf("balloc(): no more free blocks\n");
    return 0;
  }

}
/**********************END balloc*******************/

/**********************imap*******************/
imap(int fd)
{  
  char buf[BLOCK_SIZE];
  int imap;
  int ninodes;
  int nFreeInodes;
  int nFreeBlocks;
  int  i;
  printf("********************BEGIN IMAP********************\n");
 

  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;

  ninodes = sp->s_inodes_count;
  //printf("ninodes = %d\n", ninodes);

  nFreeInodes = sp->s_free_inodes_count;  
  //printf("nFreeInodes = %d\n", nFreeInodes);

  nFreeBlocks = sp->s_free_blocks_count;
  //printf("nFreeBlocks = %d\n", nFreeBlocks);

  // read Group Descriptor 0
  
  imap = getImap(fd);

  //printf("imap = %d\n", imap);
  

  // read inode_bitmap block
  get_block(fd, imap, buf);

  for (i = 1; i < ninodes + 1; i++)
  {
    (tst_bit(buf, i-1)) ? putchar('1') : putchar('0');

    if ((i % 8)==0)
    {
      if((i % 64) == 0)
      {
        printf("\n");
        continue;
      }
      printf(" ");
    }
  }
  printf("\n");
  printf("********************END IMAP********************\n");  
}
/**********************END OF imap*******************/

/**********************bmap*******************/
bmap(int fd)
{
  int bmap;
  int nBlocks;
  char buf[BLOCK_SIZE];
  int i;

  printf("********************BEGIN BMAP********************\n");

  // read SUPER block
  
  nBlocks = getNBlocks(fd);

  // printf("ninodes = %d\n", ninodes);

  // read Group Descriptor 0  
  
  bmap = getBmap(fd);
  // printf("bmap = %d\n", bmap);

  get_block(fd, bmap, buf);

  for (i=1; i < nBlocks+1; i++)
  {
    (tst_bit(buf, i-1)) ? putchar('1') : putchar('0');

    if ((i % 8) == 0)
    {
      if((i % 64) == 0)
      {
        printf("\n");
        continue;
      }
      printf(" ");
    }
  }
  printf("\n");
  printf("********************END BMAP********************\n");

}
/**********************END OF bmap*******************/


/**********************inode*******************/
inode(int fd)
{
  char buf[BLOCK_SIZE];
  int iblock;

  printf("********************BEGIN INODE********************\n");  
  // read GD
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  /****************
  printf("%8d %8d %8d %8d %8d %8d\n",
   gp->bg_block_bitmap,
   gp->bg_inode_bitmap,
   gp->bg_inode_table,
   gp->bg_free_blocks_count,
   gp->bg_free_inodes_count,
   gp->bg_used_dirs_count);
  ****************/ 
  iblock = gp->bg_inode_table;   // get inode start block#
  printf("inode_block=%d\n", iblock);

  // get inode start block     
  get_block(fd, iblock, buf);

  ip = (INODE *)buf + 1;         // ip points at 2nd INODE
  
  printf("mode = %4x ", ip->i_mode);
  printf("uid = %d  gid=%d\n", ip->i_uid, ip->i_gid);
  printf("size = %d\n", ip->i_size);
  printf("time = %s", ctime(&ip->i_ctime));
  printf("link = %d\n", ip->i_links_count);
  
  printf("i_block[0] = %d\n", ip->i_block[0]);

 /*****************************
  u16  i_mode;        // same as st_imode in stat() syscall
  u16  i_uid;                       // ownerID
  u32  i_size;                      // file size in bytes
  u32  i_atime;                     // time fields  
  u32  i_ctime;
  u32  i_mtime;
  u32  i_dtime;
  u16  i_gid;                       // groupID
  u16  i_links_count;               // link count
  u32  i_blocks;                    // IGNORE
  u32  i_flags;                     // IGNORE
  u32  i_reserved1;                 // IGNORE
  u32  i_block[15];                 // IMPORTANT, but later
 ***************************/

  printf("********************END OF INODE********************\n");  
}
/**********************END OF inode*******************/

/**********************dir*******************/
dir(int fd)
{ 
  char buf[BLOCK_SIZE];
  int iblock;

  printf("********************BEGIN DIR********************\n");  
  // read GD
  
  iblock = getIBlock(fd);   // get inode start block#  

  // get inode start block     
  get_block(fd, iblock, buf);

  ip = (INODE*)buf + 1;         // ip points at 2nd INODE
    

  int dir;
  dir = ip->i_block[0];
  get_block(fd, dir, buf);

  char* cp;
  cp = buf;

  int i = 0;
  

  while(i < BLOCK_SIZE)
  {
     dp = (DIR*)cp;


    // printf("rec_len = %d\n", dp->rec_len);
    // printf("name_len = %u\n", dp->name_len);
    // printf("file_type = %u\n", dp->file_type);

    // printf("name = ");
    if(dp->file_type == 2)
    {
      //directory
      printf("DIRECTORY ");
    }
    else
    {
      //file
      printf("FILE ");
    }

     int z = 0;
     for(z = 0; z < dp->name_len; z++)
     {
       putchar(dp->name[z]);
     }
     printf("\n");

     cp += dp->rec_len;
     i += dp->rec_len;





  }

  printf("********************END OF DIR********************\n"); 
}
/**********************END OF dir*******************/