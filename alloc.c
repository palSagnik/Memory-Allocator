#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEAP_CAPACITY 64000
#define HEAP_ALLOCATED_CAPACITY 1024
#define HEAP_FREED_CAPACITY 1024

#define TODO() \
    do {\
        fprintf(stderr , "%s:%d %s is not implemented yet\n", \
                __FILE__, __LINE__, __func__); \
        abort(); \
    } while(0);

//keep track of the size of the heap chunk
typedef struct 
{
    void *start;
    size_t size;

}Chunk;

//using a separate data structure for our metadata
typedef struct 
{
    size_t count;
    Chunk chunks[HEAP_ALLOCATED_CAPACITY];
}ChunkList;

//defining functions for our new ds
//dump

void chunkListDump (const ChunkList *list)
{
    printf("Allocated Chunks: %zu\n", list->count);
    //iterating through the metadata table of allocated chunks and dumping them
    for(size_t i = 0; i < list->count; i++)
    {
        printf(" Start: %p, Size: %zu\n", list->chunks[i].start, list->chunks[i].size);
    }
}
//find
int chunkListFind (const ChunkList *list, void *ptr)
{
    TODO();
    return -1;
}

//remove
void chunkListRemove(ChunkList *list, size_t insert)
{
    TODO();
}

//insert
void chunkListInsert (ChunkList *list, void *ptr, size_t size)
{
    TODO();
}

// our heap
char heap[HEAP_CAPACITY] = {0};
size_t heapSize = 0;

//metadata for our allocated heap chunks
//to keep track of the allocated memory, a similar thing might be required for the free memory too:)
//the table is used to keep track of the start of each chunk and their sizes
ChunkList allocatedChunks = {0};
ChunkList freedChunks = {0};

void *heapAllocate (size_t size)
{
    //taking inspo from malloc that when size is 0,
    //it either returns a NULL value or a unique pointer which,
    //can be passed to free
    if (size == 0)
        return NULL;
    
    assert(heapSize + size <= HEAP_CAPACITY);

    // starting of a free heap block
    void *ptr = heap + heapSize;
    heapSize += size;
    
    chunkListInsert(&allocatedChunks, ptr, size);
    return ptr;
}

void heapFree (void *ptr)
{
    (void) ptr;
    TODO();
}

// slide over the heap with a window of 8 bytes
// and look for allocated memory resembling a pointer to a heap block
// if we find a pointer that is unreachable, a pointer is reachable if it is pointing to another allocated block of memory on the heap
// this hence becomes are automatic garbage collector
void heapCollect ()
{
    // this should scan for pointer to the stack as well
    assert(false && "heapCollect: Not implemented");
}

int main (void)
{
    for (int i = 0; i < 100; i++)
    {
        void *p = heapAllocate(i);
        if (i%2 == 0)
            heapFree(p);
    }

    //chunkListDump();
    //heapFree(root1);

    return 0;
}
