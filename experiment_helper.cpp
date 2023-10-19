#include "mem_manager.h"
#include "experiment_helper.h"

double calculate_internal_fragmentation() {
    double total_allocated = 0; 
    double total_used = 0;     

    for (const auto& chunk : occupied_chunks) {
        total_allocated += chunk.size;
        total_used += chunk.used_size;
    }

    return total_allocated - total_used;
}

double calculate_external_fragmentation() {
    double total_free_memory = 0;

    for (const auto& chunk : free_chunks) {
        total_free_memory += chunk.size;
    }

    return total_free_memory;
}

// helper function to calculate fragmentation percentages, runs both functions
std::pair<double, double> calculate_fragmentation_percentages() {
    double internal_fragmentation = calculate_internal_fragmentation();
    double external_fragmentation = calculate_external_fragmentation();
    double total_memory = internal_fragmentation + external_fragmentation;

    return std::make_pair((internal_fragmentation / total_memory) * 100,
                          (external_fragmentation / total_memory) * 100);
}
