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
  char pathTokens[20][20];
  char* token;
  int i = 0;
  int numTokens = 0;



  if(argv[2] != NULL)
  {
    // pathname was passed in
    //determine if path is relitive or absolute
    char* path;
    path = argv[2];
    printf("path = %s\n", path);

    if(path[0] == '/')
    {
      //absolute
      isAbsolute = true;
    }
    else
    {
      //relitvie
      isAbsolute = false;
    }

    token = strtok(path, "/");
    
    while(token != NULL)
    {     
      strcpy(pathTokens[i], token);
      token = strtok(NULL, "/");      
      i++;
    }
  
    numTokens = i;
    pathTokens[i][0] = '\0';

  }

  printf("numTokens = %d\n", numTokens);  
  i = 0;

  if(isAbsolute == true)
  {
    printf("looking for absolute path\n");
  }
  else
  {
    printf("looking for relitvie path\n");
  }
  
  while(i < numTokens)
  {
    printf("pathTokens[%d] = %s\n", i, pathTokens[i]);
    i++;
  }
  printf("going to mount file system %s\n", disk);
  
  
  super(fd);
  groupDescriptor(fd);
  int InodeBeginBlock;
  InodeBeginBlock = getIBlock(fd);
  printf("Inodes begin at block %d\n", InodeBeginBlock);
  printf("get root inode and try to find 1st dir in path\n");

  char buf[BLOCK_SIZE];

  get_block(fd, InodeBeginBlock, buf);
  ip = (INODE*)buf + 1; //root is inode number 2

  printf("have root inode in buf\n");
  printf("root's contentes are:\n");
  dir(fd);




  // imap(fd);
  // bmap(fd);
  // //inode(fd);
  // dir(fd);



  

}
/*************************END OF MAIN***************************/