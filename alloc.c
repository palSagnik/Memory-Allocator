#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEAP_CAPACITY 640000
#define CHUNK_LIST_CAP 1024

#define TODO                                                 \
    do                                                       \
    {                                                        \
        fprintf(stderr, "%s:%d %s is not implemented yet\n", \
                __FILE__, __LINE__, __func__);               \
        abort();                                             \
    } while (0);

// keep track of the size of the heap chunk
typedef struct
{
    char *start;
    size_t size;

} Chunk;

// using a separate data structure for our metadata
// this will be a sorted array
// and we will be adding the new chunks at the end of memory
typedef struct
{
    size_t count;
    Chunk chunks[CHUNK_LIST_CAP];
} ChunkList;

// defining functions for our new ds

// insert
// O(list->count)
void chunkListInsert(ChunkList *list, void *start, size_t size)
{
    assert(list->count < CHUNK_LIST_CAP);
    list->chunks[list->count].start = start;
    list->chunks[list->count].size = size;

    // sorting our chunks using bubble sort
    size_t i = list->count;
    while (list->chunks[i].start < list->chunks[i - 1].start && i != 0)
    {
        const Chunk t = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i - 1] = t;

        i--;
    }

    list->count += 1;
}

//merge
//merging the fragmented mem chunks to get a bigger chunk for better deallocation
void chunkListMerge(ChunkList *dst, const ChunkList *src)
{
    dst->count = 0;
    for (size_t i = 0; i < src->count; i++)
    {
        const Chunk chunk = src->chunks[i];

        //check if dst not empty
        if (dst->count > 0)
        {
            Chunk *topChunk = &dst->chunks[dst->count - 1];
            
            //check if this is mergeable
            if (topChunk->start + topChunk->size ==  chunk.start)
            {
                //merge
                topChunk->size += chunk.size;
            }
            else
            {
                //append to dst
                chunkListInsert(dst, chunk.start, chunk.size);
            }
        }
        else
        {
            chunkListInsert(dst, chunk.start, chunk.size);
        }
    }

}
// dump
void chunkListDump(const ChunkList *list)
{
    printf("Chunks (%zu): \n", list->count);
    // iterating through the metadata table of allocated chunks and dumping them
    for (size_t i = 0; i < list->count; i++)
    {
        printf(" Start: %p, Size: %zu\n", list->chunks[i].start, list->chunks[i].size);
    }
}

// find
int chunkListFind(const ChunkList *list, void *ptr)
{
    for (size_t i = 0; i < list->count; i++)
    {
        if (list->chunks[i].start == ptr)
            return i;
    }

    return -1;
}

// remove
// O(list->count)
void chunkListRemove(ChunkList *list, size_t index)
{
    assert(index < list->count);
    for (size_t i = index; i < list->count - 1; i++)
        list->chunks[i] = list->chunks[i + 1];

    list->count -= 1;
}

// our heap
// we removed the notion of heapSize because the allocate function was not reusing the memory
char heap[HEAP_CAPACITY] = {0};

// metadata for our allocated heap chunks
// to keep track of the allocated memory
// the table is used to keep track of the start of each chunk and their sizes
ChunkList allocatedChunks = {0};
ChunkList freedChunks = {
    .count = 1,
    .chunks = {
        [0] = {.start = heap, .size = sizeof(heap)}}};
ChunkList tmpChunks = {0};

void *heapAllocate(size_t size)
{
    if (size > 0)
    {
        chunkListMerge(&tmpChunks, &freedChunks);
        freedChunks = tmpChunks;

        for (size_t i = 0; i < freedChunks.count; ++i)
        {
            const Chunk chunk = freedChunks.chunks[i];
            if (chunk.size >= size)
            {
                chunkListRemove(&freedChunks, i);

                const size_t tailSize = chunk.size - size;
                chunkListInsert(&allocatedChunks, chunk.start, size);

                if (tailSize > 0)
                {
                    chunkListInsert(&freedChunks, chunk.start + size, tailSize);
                }

                return chunk.start;
            }
        }
    }
    return NULL;
}

void heapFree(void *ptr)
{
    if (ptr != NULL)
    {
        // find the index, move it to the freed chunks table, remove it from the allocated chunks table
        int index = chunkListFind(&allocatedChunks, ptr);
        assert(index >= 0);
        chunkListInsert(&freedChunks, allocatedChunks.chunks[index].start, allocatedChunks.chunks[index].size);
        chunkListRemove(&allocatedChunks, (size_t)index);
    }
}

// slide over the heap with a window of 8 bytes
// and look for allocated memory resembling a pointer to a heap block
// if we find a pointer that is unreachable, a pointer is reachable if it is pointing to another allocated block of memory on the heap
// this hence becomes are automatic garbage collector
void heapCollect()
{
    // this should scan for pointer to the stack as well
    TODO;
}

int main(void)
{
    for (int i = 1; i < 10; i++)
    {
        void *p = heapAllocate(i);
        if (i % 2 == 0)
            heapFree(p);
    }

    chunkListDump(&allocatedChunks);
    chunkListDump(&freedChunks);

    return 0;
}
