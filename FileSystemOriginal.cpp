
// FileSystems2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include "FileIO.h"
#define _CRT_SECURE_NO_WARNINGS

void toLowerStng(char *stng){
	int i;
	for (i = 0; stng[i] != '\0'; i++)
		stng[i] = tolower(stng[i]);
}

int getCommand(char *command){
	toLowerStng(command);
	if (!strcmp("mount", command))
		return 1;
	if (!strcmp("copytofs", command))
		return 2;
	if (!strcmp("copyfromfs", command))
		return 3;
	if (!strcmp("format", command))
		return 4;
	if (!strcmp("ls", command))
		return 5;
	if (!strcmp("delete", command))
		return 6;
	if (!strcmp("exit", command))
		return 7;
	return -1;
}

void start(){
char inputCommand[1000],*token,*arg1,*arg2;
int commandNum;
while (!(GetAsyncKeyState(VK_CONTROL))){
	printf(">>");
	char* inputCommand = (char*)malloc(sizeof(char) * 100);
	gets(inputCommand);
	token = strtok(inputCommand, " ");
	commandNum = getCommand(token);
	if (commandNum == 1){
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");
		//init(arg1, atoi(arg2));
		printf("Initialised\n");
	}
	else if (commandNum == 2){
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");
		enterFile(arg1, arg2);
		printf("%s Saved\n", arg1);
	}
	else if (commandNum == 3){
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");
		getFile(arg1, arg2);
		printf("%s Created\n", arg2);
	}
	else if (commandNum == 4){
		arg1 = strtok(NULL, " ");
		format(atoi(arg1));
		printf("Disk formatted\n");
	}
	else if (commandNum == 5)
		listOfFiles();
	else if (commandNum == 6){
		arg1 = strtok(NULL, " ");
		deleteFile(arg1);
	}
	else if (commandNum == 7){
		exit(0);
	}
	else{
		printf("No such command %s\n", token);
	}
}
}

int _tmain(int argc, _TCHAR* argv[])
{
	format(0);
	start();
	return 0;
}

