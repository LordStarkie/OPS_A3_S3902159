#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <forward_list>
#include <unistd.h>


struct memory_chunk
{
    std::size_t size;
    void *space;
};

enum class Allocation_Strategy{
    FIRST_FIT,
    BEST_FIT
};

std::forward_list<memory_chunk> occupiedChunks;
std::forward_list<memory_chunk> freeChunks;

// return size closest to 32,64,128,256,512
std::size_t determine_partition_size(std::size_t chunk_size);

void* alloc(std::size_t chunk_size);
memory_chunk* first_fit_allocation(std::size_t partition_size);
memory_chunk* best_fit_allocation(std::size_t partition_size);

void dealloc(void* chunk);


#endif