#include "stdafx.h"
#include "FileIO.h"
#include <stdlib.h>
#include <string.h>
#define blockSize 16384
#define maxBlocks 100

//static int blockSize;

metaData* getMetaData(){
	void *buffer;
	metaData *meta = (metaData*)malloc(sizeof(metaData));
	buffer = getBlock(0);
	memcpy(meta, buffer, sizeof(metaData));
	return meta;
}

void setMetaData(metaData* meta){
	setBlock(0, meta);
}

int getNextBlockNum(bool *arr){
	int i;
	for (i = 0; i < 6400; i++){//TO BE CHANGED
		if (arr[i] == 1)
			return i;
	}
	return 0;
}

void enterFile(char *fileName, char *outputName){
	int size, blocksRequired, i, blockNum, *blocksOccupied;
	FILE *toBeWrittenFile = fopen(fileName, "rb");
	file *inputFile = (file*)malloc(sizeof(file));
	char* buffer = (char*)malloc(blockSize*sizeof(char));
	metaData *meta = (metaData*)malloc(sizeof(metaData));
	if (!toBeWrittenFile){
		printf("Cannot find %s\n", fileName);
		exit(0);
	}
	fseek(toBeWrittenFile, 0L, SEEK_END);
	size = ftell(toBeWrittenFile);
	blocksRequired = size / blockSize;
	if (size%blockSize != 0)
		blocksRequired++;
	strcpy(inputFile->fileName, outputName);
	inputFile->fileLength = size;
	inputFile->numOfBlocks = blocksRequired;

	meta = getMetaData();
	if (meta->magic != 9){
		printf("HDD corrupt\n");
		return;
	}
	if (meta->freeBlocks < blocksRequired){
		printf("Memory not available\n");
		return;
	}
	if (blocksRequired>maxBlocks){
		printf("File size too large\n");
		return;
	}
	blocksOccupied = (int*)malloc(maxBlocks*sizeof(int));

	for (i = 0; i < 6400; i++)
		if (meta->blocks[i])
			break;

	if (blocksRequired == 1)
		inputFile->startBlock = i;

	fseek(toBeWrittenFile, 0, SEEK_SET);

	for (i = 0; i < blocksRequired; i++){
		blockNum = getNextBlockNum(meta->blocks);
		blocksOccupied[i] = blockNum;
		fread(buffer, blockSize, 1, toBeWrittenFile);
		setBlock(blockNum, buffer);
		meta->blocks[blockNum] = 0;
	}
	if (blocksRequired > 1){
		blockNum = getNextBlockNum(meta->blocks);
		setBlock(blockNum, blocksOccupied);
		meta->blocks[blockNum] = 0;
		inputFile->startBlock = blockNum;
	}

	meta->files[meta->numOfFiles] = *inputFile;
	meta->numOfFiles += 1;
	setMetaData(meta);

	fclose(toBeWrittenFile);
}

void getFile(char *fileName, char *output){
	FILE *outputFile = fopen(output, "wb");
	int startBlock, numOfBlocks, fileLength, metaSize = sizeof(metaData), i, *blocksOccupied;
	metaData *meta;
	file *tempFile;
	char *buffer;
	blocksOccupied = (int*)malloc(maxBlocks*sizeof(int));
	meta = getMetaData();
	if (meta->magic != 9){
		printf("HDD corrupt\n");
		exit(0);
	}
	for (i = 0; i < meta->numOfFiles; i++){
		if (!strcmp(meta->files[i].fileName, fileName)){
			startBlock = meta->files[i].startBlock;
			numOfBlocks = meta->files[i].numOfBlocks;
			fileLength = meta->files[i].fileLength;
			break;
		}
	}
	if (i == meta->numOfFiles){
		printf("File %s not found\n", fileName);
		return;
	}

	if (numOfBlocks == 1){
		buffer = (char*)getBlock(startBlock);
		fwrite(buffer, fileLength, 1, outputFile);
	}
	else{
		memcpy(blocksOccupied, getBlock(startBlock), maxBlocks);
		//blocksOccupied = (int*)getBlock(startBlock);
		for (i = 0; i < numOfBlocks; i++){
			if (fileLength > blockSize){
				buffer = (char*)getBlock(blocksOccupied[i]);
				fwrite(buffer, blockSize, 1, outputFile);
				fileLength -= blockSize;
			}
			else{
				buffer = (char*)getBlock(blocksOccupied[i]);
				fwrite(buffer, fileLength, 1, outputFile);
			}
		}
	}

	fclose(outputFile);
}

void format(int block){
	int i;
	metaData *meta = (metaData*)malloc(sizeof(metaData));
	meta->magic = 9;
	meta->numOfFiles = 0;

	for (i = 0; i < 6400; i++)
		meta->blocks[i] = 1;
	meta->blocks[0] = 0;
	meta->freeBlocks = 6400;
	//printf("formatting\n");
	setBlock(0, meta);
	//meta = (metaData*)getBlock(0);
	//printf("check magic %d\n", meta->magic);
	//printf("done formatting\n");
	//blockSize = block;
}

void deleteFile(char *fileName){
	int blocksRequired = 0, metaSize = sizeof(metaData), i;
	FILE *hdd;
	long int size;
	metaData *meta;
	meta = getMetaData();
	for (i = 0; i < meta->numOfFiles; i++){
		if (!strcmp(meta->files[i].fileName, fileName)){
			meta->files[i] = meta->files[meta->numOfFiles - 1];
			meta->numOfFiles -= 1;
			setBlock(0, meta);
			printf("Deleted\n");
			return ;
		}
	}
	printf("File %s not found\n", fileName);
	return ;
}

void listOfFiles(){
	metaData *meta;
	meta = getMetaData();
	if (meta->numOfFiles == 0){
		printf("No files to show\n");
	}
	for (int i = 0; i < meta->numOfFiles; i++){
		printf("%s\n", meta->files[i].fileName);
	}
}

/*void initFileIO(char *hddName, int block){
metaData *meta;
unsigned int numOfBlocks;
blockSize = block;
numOfBlocks = init(hddName, block);
//if (!numOfBlocks)
//	format

}*/