#include <allocator.h>

std::size_t determine_partition_size(std::size_t chunk_size){

    if (chunk_size <= 32) {return 32;}
    if (chunk_size <= 64) {return 64;}
    if (chunk_size <= 128) {return 128;}
    if (chunk_size <= 256) {return 256;}
    if (chunk_size <= 512) {return 512;}

    return 512;
}

void* alloc(std::size_t chunk_size){
    // set size 
    std::size_t partition_size = determine_partition_size(chunk_size);


    // look for free list for chunk
    for (auto it = freeChunks.begin(); it != freeChunks.end(); ++it)
    {
        if (it->size == partition_size)
        {
            // move chunk from free to occupied
            occupiedChunks.push_front(*it);
            freeChunks.erase_after(freeChunks.before_begin());  
            return it->space;
        }
    }

    // if no suitable chunks are found
    void* new_space = sbrk(partition_size);
    memory_chunk new_chunk{partition_size,new_space};
    occupiedChunks.push_front(new_chunk);
    
    
}

