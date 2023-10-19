#include "mem_manager.h"

//FIRST FIT or BEST FIT
Allocation_Strategy current_strategy; 
std::forward_list<memory_chunk> occupied_chunks;
std::forward_list<memory_chunk> free_chunks;

std::size_t determine_partition_size(std::size_t chunk_size){

    if (chunk_size <= 32) {return 32;}
    if (chunk_size <= 64) {return 64;}
    if (chunk_size <= 128) {return 128;}
    if (chunk_size <= 256) {return 256;}
    if (chunk_size <= 512) {return 512;}

    return 512;
}

// main function for ALLOC
void* alloc(std::size_t chunk_size) {
    std::cout << "alloc: " << chunk_size << std::endl;

    
    // get closest chunk size
    std::size_t partition_size = determine_partition_size(chunk_size);
    memory_chunk* allocated_chunk = nullptr;

    // switch case
    switch (current_strategy) {
        case Allocation_Strategy::FIRST_FIT:
            allocated_chunk = first_fit_allocation(partition_size);
            break;

        case Allocation_Strategy::BEST_FIT:
            allocated_chunk = best_fit_allocation(partition_size);
            break;
    }

    // return if chunk allocated
    if (allocated_chunk) {
        return allocated_chunk->space;
        }

    // if no chunk found, make new and allocate
    void* new_space = sbrk(partition_size);

    // failed
    if (new_space == (void*) -1) { 
        return nullptr;
    }

    // add the new memory allocation to the occupied list
    memory_chunk new_chunk{ partition_size, new_space };
    occupied_chunks.push_front(new_chunk);
    return new_space;
}

// DEALLOC
void dealloc(void* ptr) {
    std::cout << "dealloc" << std::endl;

    auto previous_chunk = occupied_chunks.before_begin();

    // iterate through occupied chunks to find the pointer
    for (auto it = occupied_chunks.begin(); it != occupied_chunks.end(); previous_chunk = it, ++it) {
        if (it->space == ptr) {

            // Check if the chunk is the latest one allocated (i.e., contiguous with the program's break)
            if ((char*)it->space + it->size == (char*)sbrk(0)) {
                // Adjust the program's break value to release memory. This shrinks the program's memory.
                std::cout << "brk()" << std::endl;
                brk(ptr);
            } else {
                // Move it to the free list.
                memory_chunk freed_chunk = *it; // Create a copy of the chunk
                free_chunks.push_front(freed_chunk);
            }

            // Remove chunk from the occupied list.
            occupied_chunks.erase_after(previous_chunk);
            return;
        }
    }

    // If we've reached here, the pointer was not found in the occupied_chunks.
    std::cerr << "Fatal Error: Attempting to free memory that was never allocated." << std::endl;
    std::abort();  // Terminate the program.
}



// FIRST FIT function
memory_chunk* first_fit_allocation(std::size_t partition_size) {
    auto previous = free_chunks.before_begin();

    // iterate through chunks
    for (auto it = free_chunks.begin(); it != free_chunks.end(); previous = it, ++it) {
        if (it->size >= partition_size) {
            // move chunk
            occupied_chunks.push_front(*it);
            free_chunks.erase_after(previous); 
            return &*it;
        }
    }
    return nullptr; 
}

// BEST FIT function
memory_chunk* best_fit_allocation(std::size_t partition_size) {
    // pointers
    auto best_fit = free_chunks.end();
    auto best_fit_prev = free_chunks.before_begin();
    auto previous = free_chunks.before_begin();

    // iterate through chunks
    for (auto it = free_chunks.begin(); it != free_chunks.end(); previous = it, ++it) {
        if (it->size >= partition_size && 
            (best_fit == free_chunks.end() || it->size < best_fit->size)) {
            best_fit = it;
            best_fit_prev = previous;
        }
    }

    if (best_fit != free_chunks.end()) {
        // move chunk
        occupied_chunks.push_front(*best_fit);
        // remove from free
        free_chunks.erase_after(best_fit_prev); 
        return &*best_fit; 
    }
    return nullptr;
}