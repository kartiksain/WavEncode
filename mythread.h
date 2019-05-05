#include <stdbool.h>

struct ThreadData
{
	int visited;
	int TotalItems;
	int numofThreads;
	int completed;
	int noThreadsCreated;
	char *path;
	char **FileName;	
	pthread_t *pTid;
	pthread_mutex_t queue_mutex;// = PTHREAD_MUTEX_INITIALIZER;
	bool (*work_fn)(const char*,const char*);
};

void* ThreadFunction(void *arg);

bool createThreadPool(char *path,char **FilesList,int size,bool (*fn)(const char *,const char *));

void JoinThreads();
