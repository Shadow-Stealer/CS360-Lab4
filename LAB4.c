#include "utilities.h"

int fd;





/**********************main************************************/
main(int argc, char *argv[ ])
{ 
  char *disk = "fdimage";

  if (argc > 1)
  {
    disk = argv[1];
  }  
  fd = open(disk, O_RDWR);
  if (fd < 0)
  {
    printf("open failed\n");
    exit(1);
  }
  
  super(fd);
  groupDescriptor(fd);

  int InodesBeginBlock;
  InodesBeginBlock = getFirstInode(fd);

  //imap(fd);
  //bmap(fd);
  //inode(fd);
  //dir(fd);



  

}
/*************************END OF MAIN***************************/