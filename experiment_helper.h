#ifndef EXPERIMENT_HELPER
#define EXPERIMENT_HELPER

#include <list>

double calculate_internal_fragmentation(const std::forward_list<memory_chunk>& occupied_chunks);
double calculate_external_fragmentation(const std::forward_list<memory_chunk>& free_chunks);
std::pair<double, double> calculate_fragmentation_percentages(const std::forward_list<memory_chunk>& occupied_chunks, const std::forward_list<memory_chunk>& free_chunks);

#endif