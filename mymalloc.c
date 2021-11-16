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
   
    // checking to see if the pointer is empty if so call mymalloc
 if (ptr == NULL ){
        return mymalloc(size);
    }
    // checking size, if zero then call free and return NULL
    if(size == 0){
        myfree(ptr);
        return NULL;
}
    // if neither are attemted then a new space must be created
    void* temp_space = mymalloc(size);
    memcpy(temp_space, ptr, size); // using memcopy to use the pointer size and new space
    myfree(ptr);
    ptr = temp_space;
    return ptr;
}

void mycleanup(){


}

double utilization(){


}
