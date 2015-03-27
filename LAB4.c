#include "utilities.h"







/**********************main************************************/
main(int argc, char *argv[ ])
{ 
  int fd;
  char *disk = "fdimage";

  // if (argc > 1 )
  // {
  //   disk = argv[1];
  // }

  fd = open(disk, O_RDWR);
  if (fd < 0)
  {
    printf("open failed\n");
    exit(1);
  }
  
  super(fd);
  groupDescriptor(fd);

  int InodeBeginBlock;
  InodeBeginBlock = getIBlock(fd);

  char buf[BLOCK_SIZE];

  get_block(fd, InodeBeginBlock, buf);
  ip = (INODE*)buf+1; //want inode number 2







  imap(fd);
  bmap(fd);
  //inode(fd);
  //dir(fd);



  

}
/*************************END OF MAIN***************************/