#include <stdio.h>
#include <assert.h>

#define HEAP_CAPACITY 64000

// our heap
char heap[HEAP_CAPACITY] = {0};
size_t heapSize = 0;

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
    return NULL;
}

// slide over the heap with a window of 8 bytes
// and look for allocated memory resembling a pointer to a heap block
// if we find a pointer that is unreachable, a pointer is reachable if it is pointing to another allocated block of memory on the heap
// this hence becomes are automatic garbage collector
void heapCollect ()
{
    // this should scan for pointer to the stack as well
    
}
