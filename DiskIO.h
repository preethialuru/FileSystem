#include "stdafx.h"
#define fileSize 32
typedef struct metaData metaData;
typedef struct file file;

struct file{
	char fileName[20];
	unsigned int startBlock, numOfBlocks, fileLength;
};

struct metaData{
	int magic;
	file files[fileSize];
	int numOfFiles;
	int freeBlocks;
	bool blocks[6400];
};

void deleteFile(char *fileName);
void setBlock(int block, void *buffer);
void* getBlock(int block);
//unsigned int init(char *hdd, int block);