#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <forward_list>

struct memory_chunk
{
    std::size_t size;
    void *space;
};
std::forward_list<memory_chunk> occupiedChunks;
std::forward_list<memory_chunk> freeChunks;

// return size closest to 32,64,128,256,512
std::size_t determine_partition_size(std::size_t chunk_size);
void* alloc(std::size_t chunk_size);
void dealloc(void* chunk);


#endif