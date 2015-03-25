
/********* super.c code ***************/

#include "type.h"






int fd;

int get_block(int fd, int blk, char buf[ ])
{
  lseek(fd, (long)blk*BLOCK_SIZE, 0);
  read(fd, buf, BLOCK_SIZE);
}

super()
{
  char* buf[BLOCK_SIZE];
  // read SUPER block
  get_block(fd, 1, buf);  
  sp = (SUPER *)buf;

  // check for EXT2 magic number:

  printf("s_magic = %x\n", sp->s_magic);
  if (sp->s_magic != 0xEF53){
    printf("NOT an EXT2 FS\n");
    exit(1);
  }

  printf("********************BEGIN SUPER BLOCK********************\n");

  printf("EXT2 FS OK\n");



  printf("s_inodes_count = %d\n", sp->s_inodes_count);
  printf("s_blocks_count = %d\n", sp->s_blocks_count);

  printf("s_free_inodes_count = %d\n", sp->s_free_inodes_count);
  printf("s_free_blocks_count = %d\n", sp->s_free_blocks_count);
  printf("s_first_data_blcok = %d\n", sp->s_first_data_block);


  printf("s_log_block_size = %d\n", sp->s_log_block_size);
  printf("s_log_frag_size = %d\n", sp->s_log_frag_size);

  printf("s_blocks_per_group = %d\n", sp->s_blocks_per_group);
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
groupDescriptor()
{
  char* buf[BLOCK_SIZE];
  get_block(fd, 2, buf);

  gp = (GD*)buf;

  printf("********************BEGIN GROUP GROUP DESCRIPTOR BLOCK********************\n");

  printf("Bmap Block Number = %d\n", gp->bg_block_bitmap);
  printf("Imap Block Number = %d\n", gp->bg_inode_bitmap);
  printf("Inodes begin block number = %d\n", gp->bg_inode_table);
  printf("free blocks = %d\n", gp->bg_free_blocks_count);
  printf("free inodes = %d\n", gp->bg_free_inodes_count);
  printf("Ussed Directories count = %d\n", gp->bg_used_dirs_count);
  printf("********************END GROUP GROUP DESCRIPTOR BLOCK********************\n");

}
/**********************END OF groupDescriptor*******************/

/**********************tst_bit*******************/
int tst_bit(char *buf, int bit)
{
  int i, j;
  i = bit / 8;  j = bit % 8;
  if (buf[i] & (1 << j))
     return 1;
  return 0;
}
/**********************END OF tst_bit*******************/

/**********************imap*******************/
imap()
{  
  char buf[BLOCK_SIZE];
  int  imap, ninodes;
  int  i;
  printf("********************BEGIN IMAP********************\n");
 

  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;



  ninodes = sp->s_inodes_count;
  printf("ninodes = %d\n", ninodes);

  // read Group Descriptor 0
  get_block(fd, 2, buf);
  gp = (GD *)buf;

  imap = gp->bg_inode_bitmap;
  printf("imap = %d\n", imap);

  // read inode_bitmap block
  get_block(fd, imap, buf);

  for (i=1; i < ninodes+1; i++)
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
bmap()
{
  int bmap;
  int ninodes;
  char buf[BLOCK_SIZE];
  int i;

  printf("********************BEGIN BMAP********************\n");

  // read SUPER block
  get_block(fd, 1, buf);
  sp = (SUPER *)buf;
  ninodes = sp->s_inodes_count;

  printf("ninodes = %d\n", ninodes);

  // read Group Descriptor 0
  get_block(fd, 2, buf);
  gp = (GD *)buf;

  
  
  bmap = gp->bg_block_bitmap;
  printf("bmap = %d\n", bmap);

  get_block(fd, bmap, buf);

  for (i=1; i < ninodes+1; i++)
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
  printf("********************END BMAP********************\n");

}
/**********************END OF bmap*******************/


/**********************inode*******************/
inode()
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
  
  printf("mode=%4x ", ip->i_mode);
  printf("uid=%d  gid=%d\n", ip->i_uid, ip->i_gid);
  printf("size=%d\n", ip->i_size);
  printf("time=%s", ctime(&ip->i_ctime));
  printf("link=%d\n", ip->i_links_count);
  printf("i_block[0]=%d\n", ip->i_block[0]);

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
dir()
{ 
  char buf[BLOCK_SIZE];
  int iblock;

  printf("********************BEGIN DIR********************\n");  
  // read GD
  get_block(fd, 2, buf);
  gp = (GD *)buf;
  
  iblock = gp->bg_inode_table;   // get inode start block#
  

  // get inode start block     
  get_block(fd, iblock, buf);

  ip = (INODE *)buf + 1;         // ip points at 2nd INODE
    

  int dir;
  dir = ip->i_block[0];
  get_block(fd, dir, buf);

  char* cp;
  cp = buf;

  int i = 0;
  

  while(i < BLOCK_SIZE)
  {
    dp = (DIR*)cp;


    printf("rec_len = %d\n", dp->rec_len);
    printf("name_len = %u\n", dp->name_len);
    printf("file_type = %u\n", dp->file_type);
    printf("name = %s\n", dp->name);

    cp += dp->rec_len;
    i += dp->rec_len;

  }



  printf("********************END OF DIR********************\n"); 
}
/**********************END OF dir*******************/

/**********************main************************************/
main(int argc, char *argv[ ])
{ 
  char *disk = "mydisk";

  if (argc > 1)
    disk = argv[1];
  fd = open(disk, O_RDONLY);
  if (fd < 0){
    printf("open failed\n");
    exit(1);
  }

  super();
  groupDescriptor();
  imap();
  bmap();
  inode();
  dir();

  


}
/*************************END OF MAIN***************************/