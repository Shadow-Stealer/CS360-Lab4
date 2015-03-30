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
  char pathTokens[PATH_MAX][EXT2_NAME_LEN];
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
      /**************************************
        TODO:
        add cwd to path tokens before tokenizing argv[2]
      ***************************************/
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
  printf("root's contents are:\n");
  list_dir(fd);


  int iblock;


  iblock = InodeBeginBlock; // returns 5 ... inodes begin at block 5

  

  int inode = 2; //start at root
  int tokenNumber = 0;
  bool isDir = false;

  while(tokenNumber < numTokens)
  {                // 0     5      2          "tiny"
    inode = find_dir(fd, iblock, inode, &isDir, pathTokens[tokenNumber]);
    if(inode == 0)
    {
      printf("didn't find it\n");
      break;
    }
    else
    {
      //if(inode > 8) we need to look in the seccond inode block
      //if(inode > 16) we need to look in the third inode block
      //...
      //there are 23 inode blocks
      
      if(isDir == false && tokenNumber != numTokens -1)
      {
        //found something with same name
        //but is not a directory
        printf("not a directory\n");
        return;
      }


      //iblock = (inode / 8) + InodeBeginBlock;
      iblock = InodeBeginBlock;
      tokenNumber++;
    }

  }
  if(inode == 0)
  {
    printf("Didn't find it...returning\n");
    return;
  }
  else
  {
    printf("found it\n");
    printf("its inode number is: %d\n",inode);
  }







  // imap(fd);
  // bmap(fd);
  // //inode(fd);
  // dir(fd);



  

}
/*************************END OF MAIN***************************/