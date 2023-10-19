#ifndef EXPERIMENT_HELPER
#define EXPERIMENT_HELPER

#include <list>

double calculate_internal_fragmentation();
double calculate_external_fragmentation();
std::pair<double, double> calculate_fragmentation_percentages();

#endif