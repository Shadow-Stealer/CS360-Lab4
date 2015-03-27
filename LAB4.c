#include "utilities.h"







/**********************main************************************/
main(int argc, char *argv[ ])
{ 
  int fd;
  char *disk = "fdimage";

  if (argc > 1 )
  {
    disk = argv[1];
  }
  if(strcmp(argv[1], "-d") == 0)
  {
    disk = "fdimage";
  }


  fd = open(disk, O_RDWR);
  if (fd < 0)
  {
    printf("open failed\n");
    exit(1);
  }
  bool isAbsolute = false;
  char* pathTokens [20];
  char* token;
  int i = 0;
  int numTokens = 0;



  if(argv[2] != NULL)
  {
    // pathname was passed in
    //determine if path is relitive or absolute

    if(argv[2][0] == '/')
    {
      //absolute
      isAbsolute = true;
    }
    else
    {
      //relitvie
      isAbsolute = false;
    }

    token = strtok(argv[2], "/");
    

    while(token != NULL)
    {
      strcpy(token, pathTokens[i]);
      token = strtok(NULL, "/")
      i++;
    }
    numTokens = i;


  }
  
  super(fd);
  groupDescriptor(fd);

  int InodeBeginBlock;
  InodeBeginBlock = getIBlock(fd);

  char buf[BLOCK_SIZE];

  get_block(fd, InodeBeginBlock, buf);
  ip = (INODE*)buf + 1; //root is inode number 2

  imap(fd);
  bmap(fd);
  //inode(fd);
  dir(fd);



  

}
/*************************END OF MAIN***************************/