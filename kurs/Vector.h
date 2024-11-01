#ifndef VECTOR_H
#define VECTOR_H

#include <cstdint>
#include <vector>

class Vector {
public:
    Vector(uint32_t size, const std::vector<uint16_t>& values)
        : size(size), values(values) {}

    uint32_t size;
    std::vector<uint16_t> values;
};

#endif // VECTOR_H
