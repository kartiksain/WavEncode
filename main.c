#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "ListFilesInArray.h"
#include "WavReadEncode.h"
#include "mythread.h"


bool WavConvert(const char *path,const char *filename)
{
	char *fileNameWithPath=NULL;
	int sizePath=strlen(path);
	int sizeFileName=strlen(filename);
	bool bRet=false;
	fileNameWithPath=(char *)malloc(sizePath + sizeFileName + 1);
        if(fileNameWithPath!=NULL)
	{
		memset(fileNameWithPath,'\0',sizePath + sizeFileName +1);
		strncpy(fileNameWithPath,path,sizePath);
		strncpy(fileNameWithPath+sizePath,filename,sizeFileName);
		printf("mypath=%s and filename=%s\n",path,fileNameWithPath);
		bRet = WavReadEncode(fileNameWithPath);
		free(fileNameWithPath);
		fileNameWithPath=NULL;
	}
	return bRet;
}


int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Specify Input/Output Directory for this program=%s",argv[0]);
		return 0;
	}
	char **FilesList=NULL;//Created Array of FilesName
        long int *EachFileSizeInList=NULL;//Created FileSize List in Array of FilesList
	int len=0;

	char* loc=argv[1];//Location of Directory

	int size=strlen(loc);
	
	if(strncmp(loc+size-1,"/",1)!=0)
	{
		printf("Slash Missed in path\n");
		return 0;
	}
	FilesList=createArrayList(loc,&EachFileSizeInList,&len);
	if(len==0)
	{
		printf("len is zero as no wav file present in directory with .wav");
		return 0;
	}
        createThreadPool(loc,FilesList,len,WavConvert);
	JoinThreads();//It will hold Main function untill all threads tasks not completed

	deleteArrayList(FilesList,EachFileSizeInList,len);
	
	return 0;
}
