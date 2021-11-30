#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ctype.h>
#include <errno.h>

// for size and to ensure no memory leaks
#define MEM_SIZE 5000
// creating the tempBlock the same size of the mem size 
static char tempBlock[5000];

typedef struct mymalloc_memory memspot; // using a typedef because it makes delcareing much easier when going thru functions 
struct mymalloc_memory{
 char free_block; // keeping trackof which block is being freed
 int size; // size of the block 
 memspot* nextBlock; // Pointer to lead us to the next slot in memory 
}; 
// The goal of this to check the space and size required to malloc
//if 
void* mymalloc(int size){
	//on first malloc findind the amount of space to be used
	void* storageSpace;
    // checking to see the nul character also due to finding null in the first created space
	if (tempBlock[0] == '\0'){
        // create a holder to see which memory space is being executedd 
		memspot currentAllocated_space;
        // if  the amount of space is too much than allowed throw an error
        // if the size wanting to be allocated is greater than te bounds then throw an error 
		currentAllocated_space.size = size + sizeof(memspot) >= MEM_SIZE ? fprintf(stderr,"Not in bounds\n") : size;
		currentAllocated_space.free_block = '\?'; //set the freed block to escape format output, in order to format the freed block 
		currentAllocated_space.nextBlock = NULL; // set equal to Null 
		//store first memspot struct in address of first space/slot in the array 

		memcpy(&tempBlock[0],&currentAllocated_space,sizeof(memspot));

		//get addy of first free block after exectuting memspot and return the storageSpace
		storageSpace = (void*)(&tempBlock[0] + sizeof(memspot));
		return storageSpace;
        // just send back the pointer
	}
	else {
		// need to create a temp pointer to be used as the space in line to be placed
		memspot* temp = (memspot*)&tempBlock[0];
        
		long placement;
        // have to check that the something exists
		while(temp != NULL){
			//checking the free space of memory so that the temp block can be placed inside of it 
			if ((temp -> free_block != '\?') && temp -> nextBlock && (temp -> nextBlock -> free_block != '\?')){
				temp -> size += temp -> nextBlock-> size +  sizeof(memspot); // add on the size onto the next block 
				temp -> nextBlock = temp -> nextBlock -> nextBlock; // placing the space into the next block 
			}
			//if block is already free, don't create new memspots, j update and return the ptr
			if ((temp -> free_block != '\?') && (temp -> size <= size + sizeof(memspot))){
				temp -> size = size;
				temp -> free_block = '\?'; // formating freed block 
                // have to convert the temp to a long to ensure the plaecment slot
				placement = ((long)(temp) - (long)&tempBlock[0]) + sizeof(memspot);
                // return the space of the temp block 
				return (void*)(&tempBlock[placement]);
				}
            // creating a slot for the last spot on the array for allocating space 
            // using t/f statement 
			memspot* endof_Openspace = temp -> nextBlock != NULL ? temp -> nextBlock : (memspot*)&tempBlock[MEM_SIZE]; // making sure no space of nextblock is empty/NUll
            // a place holder to see where the temp is going to go depeding on size 
			placement = ((long)(temp) - (long)&tempBlock[0]) + temp -> size + sizeof(memspot);

			// if block requested fits inbetween currently allocated block and next block, make new metadata and return pointer to allocated block
			if ((void*)&tempBlock[placement] - (void*)(endof_Openspace) > (sizeof(memspot) + size)){
				//insert new pointer 
				memspot newAllocated_space; // this will be worked on and hold the sizes
				newAllocated_space.size = size;
				newAllocated_space.free_block =' \?'; // same thing as the presoiuv temp 
				newAllocated_space.nextBlock = temp -> nextBlock; // placing temp into nextb

				//place memspot in array after being allocated
				memcpy(&tempBlock[placement],&newAllocated_space,sizeof(memspot));

				temp -> nextBlock = (memspot*)&tempBlock[placement];
				
				//return pointer to first empty byte after memspot
				return (void*)(&tempBlock[placement + sizeof(memspot)]);
			}
			else {// placing temp into other 
				if (temp -> nextBlock){
					temp = temp -> nextBlock;
				}
				else // print that no more space can be added 
					fprintf(stderr, "out of space\n");
			}
		} 
	}
	return NULL;
}

void myfree(void* ptr){
	//throw an error if the memory being used it outside the range that was already defined 
    // also comparing the ptr and making sure it is a certain size 
	if((char*)ptr > &tempBlock[MEM_SIZE] || (char*)ptr < &tempBlock[0])

	{   // if not enough spare or exceeding we need to throw an error 
		fprintf(stderr, "heap overflow\n");
		return;
	}

	//make pointer to first space of memspot matching the freed memory 
	memspot* temp = (memspot*)(ptr - sizeof(memspot));
	
	//print error if already free memory is attempting to be be freed
	if(temp -> free_block == '\?')
	{
		temp -> free_block = 1;
	}
	else // too much mem would be allocated to overflow error 
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
