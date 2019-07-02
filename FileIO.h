#include "DiskIO.h"

metaData* getMetaData();
void setMetaData(metaData* meta);
void enterFile(char *fileName, char *outputFile);
void getFile(char *fileName, char *output);
void format(int block);
void deleteFile(char *fileName);
void listOfFiles();
