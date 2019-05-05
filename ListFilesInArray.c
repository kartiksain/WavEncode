
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>

#include "ListFilesInArray.h"

int getTotalFilesCount(const char *pLoc)
{
	DIR *pDirectory;
  	int FileCount = 0;
  	struct dirent *pDirentStruc;     
  	pDirectory = opendir (pLoc);
  	if (pDirectory != NULL)
  	{
    		while ((pDirentStruc = readdir (pDirectory))!=NULL )
		{
			if(strstr(pDirentStruc->d_name,".wav"))
			{
				printf("file name in count =%s\n",pDirentStruc->d_name);
      		       	FileCount++;
			}
		}
    		(void) closedir (pDirectory);
  	}
  	else
	{
		printf ("Couldn't open the directory\n");
	}
        printf("File Count of wavs in directory=%d\n",FileCount);
	return FileCount;

}

char ** createArrayList(const char *pLoc,long int **pArrSize,int *size)
{
	int CountFiles=0;
	
	int MAXLengthName=255;

	char **dpArrayList=NULL;
	long int *pArrayFileSize=NULL;
	CountFiles=getTotalFilesCount(pLoc);
        *size=CountFiles;
	if(CountFiles)
	{

		dpArrayList=(char **)malloc(CountFiles * sizeof(char *));
		pArrayFileSize=(long int *)malloc(CountFiles * sizeof(long int));

		for(int idx=0;idx<CountFiles;idx++)
		{
			dpArrayList[idx]=(char *)malloc(MAXLengthName);
		}

		DIR *pDirectory;
    		struct dirent *pDirentStruc;
    		pDirectory = opendir(pLoc);
		int index=0;
    		if (pDirectory)
    		{
        		while ((pDirentStruc = readdir(pDirectory)) != NULL /*&& index<CountFiles*/)
        		{
				if(strstr(pDirentStruc->d_name,".wav") )
				{
					printf("file name when creating list =%s\n",pDirentStruc->d_name);
					
					char *temp=NULL;
					temp=(char *)malloc(strlen(pLoc)+strlen(pDirentStruc->d_name)+1);
					memset(temp,'\0',sizeof(temp));
					strncpy(temp,pLoc,strlen(pLoc));
					strncpy(temp+strlen(pLoc),pDirentStruc->d_name,strlen(pDirentStruc->d_name));
				
					struct stat mylstat;
		                	stat(temp,&mylstat);
				
					strncpy(dpArrayList[index],pDirentStruc->d_name,strlen(pDirentStruc->d_name)+1);
					pArrayFileSize[index]=mylstat.st_size;
					index++;
					free(temp);
					temp=NULL;
				}
        		}
        		closedir(pDirectory);
    		}
		else
		{
			printf("Opendir failed =%s\n",pLoc);
			deleteArrayList(dpArrayList,pArrayFileSize,CountFiles);
			dpArrayList=NULL;
			pArrayFileSize=NULL;
			*size=0;
		}


	}
	else
	{
		printf("No wav files Found\n");

	}
        *pArrSize=pArrayFileSize;
	return dpArrayList;

}

void deleteArrayList(char **FileList,long int *FileSize,int len)
{
	if(FileSize)
	{
		free(FileSize);
	}

	if(FileList)
	{
		for(int idx=0;idx<len;idx++)
		{
			if(FileList[idx])
			{
				free(FileList[idx]);
			}
		}
		free(FileList);
	}
}
