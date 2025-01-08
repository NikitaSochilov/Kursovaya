#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

int main() {
    // Создание векторов данных
    std::vector<std::vector<uint16_t>> vectors = {
        {100, 200, 300}, // Вектор 1
        {400, 500},      // Вектор 2
        {600, 700, 800, 900, 1000} // Вектор 3
    };

    // Открытие файла для записи в двоичном формате
    std::ofstream outfile("input.bin", std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // Запись количества векторов
    uint32_t numVectors = vectors.size();
    outfile.write(reinterpret_cast<const char*>(&numVectors), sizeof(numVectors));

    // Запись каждого вектора
    for (const auto& vec : vectors) {
        uint32_t vectorSize = vec.size();
        outfile.write(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize)); // Запись размера вектора
        outfile.write(reinterpret_cast<const char*>(vec.data()), vectorSize * sizeof(uint16_t)); // Запись значений
    }

    // Закрытие файла
    outfile.close();
    std::cout << "Binary file 'input.bin' created successfully." << std::endl;

    return 0;
}