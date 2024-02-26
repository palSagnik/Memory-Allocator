#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define HEAP_CAPACITY 64000
#define HEAP_ALLOCATED_CAPACITY 1024

//keep track of the size of the heap chunk
typedef struct 
{
    void *start;
    size_t size;

}HeapChunk;

// our heap
char heap[HEAP_CAPACITY] = {0};
size_t heapSize = 0;

//metadata for our allocated heap chunks
//to keep track of the allocated memory, a similar thing might be required for the free memory too:)
HeapChunk heapAllocated[HEAP_ALLOCATED_CAPACITY] = {0};
size_t heapAllocatedSize = 0;

void *heapAllocate (size_t size)
{
    assert(heapSize + size <= HEAP_CAPACITY);

    // starting of a free heap block
    void *result = heap + heapSize;
    heapSize += size;
    return result;

}

void heapFree (void *ptr)
{
    (void) ptr;
    assert(false && "heapFree: Not implemented");
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
    char *root = heapAllocate(26);
    for (int i = 0; i < 26; i++)
    {
        root[i] = 'A' + i;
    }

    heapFree(root);

    return 0;
}
