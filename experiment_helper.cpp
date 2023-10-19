#include "mem_manager.h"
#include "experiment_helper.h"

double calculate_internal_fragmentation(const std::forward_list<memory_chunk>& occupied_chunks) {
    double total_fragmentation = 0;

    for (const auto& chunk : occupied_chunks) {
        total_fragmentation += chunk.size - chunk.used_size;
    }

    return total_fragmentation;
}

double calculate_external_fragmentation(const std::forward_list<memory_chunk>& free_chunks) {
    double total_free_memory = 0;

    for (const auto& chunk : free_chunks) {
        total_free_memory += chunk.size;
    }

    return total_free_memory;
}

// runs both other functions
std::pair<double, double> calculate_fragmentation_percentages(const std::forward_list<memory_chunk>& occupied_chunks, const std::forward_list<memory_chunk>& free_chunks) {
    // sum of chunks
    double total_memory = 0; 
    for (const auto& chunk : occupied_chunks) {
        total_memory += chunk.size;
    }
    for (const auto& chunk : free_chunks) {
        total_memory += chunk.size;
    }

    double internal_frag = calculate_internal_fragmentation(occupied_chunks);
    double external_frag = calculate_external_fragmentation(free_chunks);

    double internal_percentage = (internal_frag / total_memory) * 100;
    double external_percentage = (external_frag / total_memory) * 100;

    return { internal_percentage, external_percentage };
}
