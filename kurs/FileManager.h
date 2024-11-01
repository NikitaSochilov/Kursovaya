#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <cstdint>
#include "Vector.h"

class FileManager {
public:
    static std::vector<Vector> read_vectors(const std::string& filename);
    static void write_results(const std::string& filename, const std::vector<uint16_t>& results);
};

#endif // FILE_MANAGER_H
