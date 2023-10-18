#include "mem_manager.h"

//FIRST FIT or BEST FIT
Allocation_Strategy currentStrategy; 

// main function for ALLOC
void* alloc(std::size_t chunk_size) {
    // get closest chunk size
    std::size_t partition_size = determine_partition_size(chunk_size);
    memory_chunk* allocated_chunk = nullptr;

    // switch case
    switch (currentStrategy) {
        case Allocation_Strategy::FIRST_FIT:
            allocated_chunk = first_fit_allocation(partition_size);

            // return if chunk allocated
            if (allocated_chunk) {
                return allocated_chunk->space;
            }

        case Allocation_Strategy::BEST_FIT:
            allocated_chunk = best_fit_allocation(partition_size);

            // return if chunk allocated
            if (allocated_chunk) {
                 return allocated_chunk->space;
            }
    }

    // if no chunk found, make new and allocate
    void* new_space = sbrk(partition_size);

    // failed
    if (new_space == (void*) -1) { 
        return nullptr;
    }

    // add the new memory allocation to the occupied list
    memory_chunk new_chunk{ partition_size, new_space };
    occupiedChunks.push_front(new_chunk);
    return new_space;
}

// DEALLOC function
void dealloc(void* chunk_space) {
    auto prev_it = occupiedChunks.before_begin();

    // iterate to find chunk_space
    for (auto it = occupiedChunks.begin(); it != occupiedChunks.end(); prev_it = it, ++it) {
        if (it->space == chunk_space) {
            // move chunk to free
            freeChunks.push_front(*it);
            // erase from occupied
            occupiedChunks.erase_after(prev_it); 
            return;
        }
    }
    // abort if not found
    abort(); 
}

// FIRST FIT function
memory_chunk* first_fit_allocation(std::size_t partition_size) {
    auto previous = freeChunks.before_begin();

    // iterate through chunks
    for (auto it = freeChunks.begin(); it != freeChunks.end(); previous = it, ++it) {
        if (it->size >= partition_size) {
            // move chunk
            occupiedChunks.push_front(*it);
            freeChunks.erase_after(previous); 
            return &*it;
        }
    }
    return nullptr; 
}

// BEST FIT function
memory_chunk* best_fit_allocation(std::size_t partition_size) {
    // pointers
    auto best_fit = freeChunks.end();
    auto best_fit_prev = freeChunks.before_begin();
    auto previous = freeChunks.before_begin();

    // iterate through chunks
    for (auto it = freeChunks.begin(); it != freeChunks.end(); previous = it, ++it) {
        if (it->size >= partition_size && 
            (best_fit == freeChunks.end() || it->size < best_fit->size)) {
            best_fit = it;
            best_fit_prev = previous;
        }
    }

    if (best_fit != freeChunks.end()) {
        // move chunk
        occupiedChunks.push_front(*best_fit);
        // remove from free
        freeChunks.erase_after(best_fit_prev); 
        return &*best_fit; 
    }
    return nullptr;