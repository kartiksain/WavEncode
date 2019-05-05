#include<pthread.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include "mythread.h"

#ifdef __linux__
	#include <sys/sysinfo.h>
#else
	#include<windows.h>
#endif

struct ThreadData objThreadData;


bool createThreadPool(char *path,char **FilesList,int size,bool (*fn)(const char *,const char *))
{
	memset(&objThreadData,'\0',sizeof(objThreadData));

	objThreadData.TotalItems=size;
	objThreadData.path=path;
	objThreadData.FileName=FilesList;

	objThreadData.visited=0;
#ifdef __linux__
	objThreadData.numofThreads = get_nprocs();
#else
	objThreadData.numofThreads = pthread_num_processors_np();
#endif
	
	
	if(objThreadData.TotalItems <= objThreadData.numofThreads)
	{
		objThreadData.numofThreads=objThreadData.TotalItems;
	}

	objThreadData.noThreadsCreated=0;
	objThreadData.pTid=NULL;
	printf("no of threads=%d\n",objThreadData.numofThreads);

	pthread_mutex_init(&objThreadData.queue_mutex,NULL);

	if(objThreadData.TotalItems==0)
	return false;

	objThreadData.pTid=(pthread_t *)malloc(objThreadData.numofThreads*sizeof(pthread_t));

	if(objThreadData.pTid==NULL)
	return false;

	objThreadData.work_fn=fn;
	
        
	for (int idx=0;idx<objThreadData.numofThreads;idx++)
	{
        	int res = pthread_create(&objThreadData.pTid[idx], NULL, ThreadFunction, NULL);
        	switch (res) 
		{
        	case 0:
        	    break;
        	case EAGAIN:
            	printf("Insufficient resources to create thread. and idx=%d\n",idx);

            	break;
        	case EINVAL:
            	printf("Invalid settings in attr. and idx=%d\n",idx);

            	break;
				case EPERM:
            	printf("Insufficitent permissions. and idx=%d\n",idx);
            	break;
        	default:
            	printf("Unknown error creating thread. and idx=%d\n",idx);
        	}
		objThreadData.noThreadsCreated++;
    	}

	return true;

}


void* ThreadFunction(void *arg)
{
/*    auto *ctx = (ThreadPool*)arg;
*/
    while (1) {
        pthread_mutex_lock(&objThreadData.queue_mutex);
        if (objThreadData.visited==objThreadData.TotalItems) {
            pthread_mutex_unlock(&objThreadData.queue_mutex);
            break;
        }
	char *path;
	char *filename;
	path=objThreadData.path;
	filename=objThreadData.FileName[objThreadData.visited];		
	objThreadData.visited++;
        pthread_mutex_unlock(&objThreadData.queue_mutex);
        if (objThreadData.work_fn(path,filename)) // Increase counter on success
            objThreadData.completed++;
    }
    return NULL;
}

void JoinThreads()
{
	if(objThreadData.pTid!=NULL)
	{
		for (int idx=0;idx<objThreadData.noThreadsCreated;idx++) 
		{
        		pthread_join(objThreadData.pTid[idx], NULL);
    		}
		free(objThreadData.pTid);
		objThreadData.pTid=NULL;
		pthread_mutex_destroy(&objThreadData.queue_mutex);
		printf("Total Items sent for  encoding=%d\n",objThreadData.TotalItems);
		printf("Total Items Visited for encoding=%d\n",objThreadData.visited);
		printf("Total Items Success  encoded=%d\n",objThreadData.completed);

	}
	
}
