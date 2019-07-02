#ifndef _DISKIO_H
#define _DISKIO_H
#endif

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

//#define blockSize 16384

static int blockSize = 16384;
static char hddName[100] = "hardDisk.hdd";

void setBlock(int block, void *buffer){
	FILE *hdd;
	hdd = fopen(hddName, "rb+");
	if (!hdd){
		printf("Cannot find hdd\n");
		exit(0);
	}
	fseek(hdd, (block * blockSize), SEEK_SET);
	fwrite(buffer, 1, blockSize, hdd);
	fclose(hdd);
}

void* getBlock(int block){
	void *buffer = (void*)malloc(blockSize);
	FILE *hdd = fopen(hddName, "rb");
	if (!hdd){
		printf("Cannot find hdd\n");
		exit(0);
	}

	fseek(hdd, (block*blockSize), SEEK_SET);
	fread(buffer, 1, blockSize, hdd);
	fclose(hdd);
	return buffer;
}

/*
unsigned int init(char *hddFile,int block){
FILE *hdd = fopen(hddFile, "rb");
if (!hdd){
printf("%s not found\n");
return 0;
}
blockSize = block;
strcpy(hddName, hddFile);
fclose(hdd);
return (100 * 1024 * 1024) / block;
}*/