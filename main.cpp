#include <iostream>
#include <fstream>
#include <stack>
#include <string>

#include "mem_manager.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <datafile>" << std::endl;
        // exit failure
        return 1;  
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    // set allocation strategy
    std::string program = argv[0];
    if (program == "./firstfit") {
        current_strategy = Allocation_Strategy::FIRST_FIT;
    } else if (program == "./bestfit") {
        current_strategy = Allocation_Strategy::BEST_FIT;
    } else {
        std::cerr << "Unknown program name. Expected ./firstfit or ./bestfit" << std::endl;
        return 1;
    }

    std::stack<void*> allocated_pointers;

    // allocating and deallocating by line
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("alloc:") != std::string::npos) {
            // parsing alloc
            int size = std::stoi(line.substr(7));
            void* allocated_ptr = alloc(size);
            if (allocated_ptr) {
                allocated_pointers.push(allocated_ptr);
            }
        } else if (line == "dealloc") {
            if (!allocated_pointers.empty()) {
                void* ptr_deallocate = allocated_pointers.top();
                allocated_pointers.pop();
                dealloc(ptr_deallocate);
            } else {
                std::cerr << "nothing to deallocate" << std::endl;
            }
        }
    }

    file.close();

    // print allocs and deallocs
    std::cout << "Allocated List:" << std::endl;
    for (const auto& chunk : occupied_chunks) {
        std::cout << "Address: " << chunk.space << ", Total Size: " << chunk.size << std::endl;  

    std::cout << "\nFree List:" << std::endl;
    for (const auto& chunk : free_chunks) {
        std::cout << "Address: " << chunk.space << ", Total Size: " << chunk.size << std::endl;
    }

    // exit_success
    return 0;  
    }
}
