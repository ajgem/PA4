#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
// for size and to ensure no memory leaks
#define MEM_SIZE 5000
// define headers
//#define malloc(x) mymalloc(x)
//#define free(x) myfree(x)

static char tempBlock[5000];

/*
process for malloc:
	if block is first to be malloced:
		mem requested > mem available ? err : allocate mem and return pointer to first block after metadata:

	else:
		get first meta data, check m1 size against m2 address and find how much empty space there is.
			if the second block and its metadata fits in the empty space, place it there, update next pointers of previous meta data
			else proceede to next meta data.
			if metadata.next = null compare size of that meta data with address of last block in array.  
			if there is room for requested space, return pointer to block, else err

process for free:
	TBD


*/
typedef struct mymalloc_memory memspot;
//typedef struct metadata meta;
struct mymalloc_memory{

 char free_block; // keeping trackof which block is being freed
 int size; // size of the block 
 memspot* nextBlock; // Pointer to lead us to the next slot in memory 
 //0 if used, 1 if free
}; 

void* mymalloc(int size){
	//on first malloc
	void* storageSpace;
	if (tempBlock[0]=='\0'){
        // create a holder to see which memory space is being executedd 
		memspot currentAllocated_space;
        // if  the amount of space is too much than allowed throw an error
		currentAllocated_space.size = size + sizeof(memspot) >= MEM_SIZE ? fprintf(stderr,"Not in bounds\n") : size;
		currentAllocated_space.free_block = '\?';
		currentAllocated_space.nextBlock = NULL;
		//store first metadata struct in address of first block in array

		memcpy(&tempBlock[0],&currentAllocated_space,sizeof(memspot));

		//get address of first free block after metadata and return that as void*
		storageSpace = (void*)(&tempBlock[0] + sizeof(memspot));
		return storageSpace;

	}
	else {
		//start with a pointer to first metadata
		memspot* temp = (memspot*)&tempBlock[0];

		long placement;
		while(temp != NULL){
			//consoladate adjecent free blocks of memory
			if ((temp -> free_block != '\?') && temp -> nextBlock && (temp -> nextBlock -> free_block != '\?')){
				temp -> size += temp -> nextBlock-> size +  sizeof(memspot);
				temp -> nextBlock = temp -> nextBlock -> nextBlock;
			}
			//if block is already free, no need to create new metadata, just update current one and return pointer. 
			if ((temp -> free_block != '\?')&&(temp -> size <= size + sizeof(memspot))){
				temp -> size = size;
				temp -> free_block = '\?';
				placement = ((long)(temp) - (long)&tempBlock[0]) + sizeof(memspot);
				return (void*)(&tempBlock[placement]);
				}

			memspot* currentMemSpaceEnd = temp -> nextBlock != NULL ? temp -> nextBlock : (memspot*)&tempBlock[MEM_SIZE];

			placement = ((long)(temp) - (long)&tempBlock[0]) + temp -> size + sizeof(memspot);

			// if block requested fits inbetween currently allocated block and next block, make new metadata and return pointer to allocated block
			if ((void*)&tempBlock[placement] - (void*)(currentMemSpaceEnd) > (sizeof(memspot) + size)){
				//insert new pointer 
				memspot newAllocated_space; // this will be worked on and hold the sizes
				newAllocated_space.size = size;
				newAllocated_space.free_block =' \?';
				newAllocated_space.nextBlock = temp -> nextBlock;

				//place metadata in array directly after the allocated block
				memcpy(&tempBlock[placement],&newAllocated_space,sizeof(memspot));

				temp -> nextBlock = (memspot*)&tempBlock[placement];
				
				//return pointer to first empty byte after metadata
				return (void*)(&tempBlock[placement + sizeof(memspot)]);
			}
			else {
				if (temp -> nextBlock){
					temp = temp -> nextBlock;
				}
				else 
					fprintf(stderr, "out of space\n");
			}
		} 
	}
	return NULL;
}

void myfree(void* ptr){
	//print error if user is attempting to free a block of memory that is outside the bounds of the simulated "memory"
	//unsure if the address of mem will actually be withing the range of 0-5000 or if it will have a legitimate address according to actual memory management

	if((char*)ptr > &tempBlock[MEM_SIZE] || (char*)ptr < &tempBlock[0])

	{
		fprintf(stderr, "heap overflow\n");
		return;
	}

	//make pointer to first byte of metadata corresponding to memory to be freed
	memspot* temp = (memspot*)(ptr - sizeof(memspot));
	
	//print error if already free memory is attempting to be be freed
	if(temp -> free_block == '\?')
	{
		temp -> free_block = 1;
	}
	else
	{
		fprintf(stderr, "heap overflow\n");
		return;
	}
	
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

/*
int main(int argc, char **argv){
char* test = (char*)malloc(sizeof(char)*4);
printf("%p\n",(void*)test);
char* test2 = (char*)malloc(sizeof(char)*4);
printf("%p\n",(void*)test2);
char* test3 = (char*)malloc(sizeof(char)*4);
printf("%p\n",(void*)test3);

free(test2);
char* test4 = (char*)malloc(sizeof(char)*4);
printf("%p\n",(void*)test4);
return 0;
}*/
