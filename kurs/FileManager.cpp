#include "FileManager.h"
#include <fstream>
#include <stdexcept>

std::vector<Vector> FileManager::read_vectors(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open input file");
    }
    
    uint32_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(uint32_t));
    std::vector<Vector> vectors;

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
        std::vector<uint16_t> values(size);
        file.read(reinterpret_cast<char*>(values.data()), size * sizeof(uint16_t));
        
        vectors.emplace_back(size, values);
    }

    return vectors;
}

void FileManager::write_results(const std::string& filename, const std::vector<uint16_t>& results) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open results file");
    }

    uint32_t count = results.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));
    file.write(reinterpret_cast<const char*>(results.data()), count * sizeof(uint16_t));
}
