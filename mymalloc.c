#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void myinit(int allocAlg);
void* mymalloc(size_t size);
void myfree(void* ptr);
void* myrealloc(void* ptr, size_t size);
void mycleanup();
double utilization();


void myinit(int allocAlg){


}

void* mymalloc(size_t size){

    if(size == 0){
        return NULL;
    }

}

void myfree(void* ptr){
    if(!ptr)
        return;
}

void* myrealloc(void* ptr, size_t size){
    if (ptr == NULL ){
        return mymalloc(size);
    }
    if(size == 0){
        myfree(ptr);
        return NULL;
else{
    return NULL;
}
}

void mycleanup(){


}

double utilization(){


}
