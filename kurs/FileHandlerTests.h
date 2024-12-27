class TestFileHandler : public suite {
public:
    TestFileHandler()
        : suite("TestFileHandler", "Тесты для работы с файлами") {
        add("Чтение корректного файла", testcase(this, "ReadCorrectFile",
                                                 &TestFileHandler::test_read_correct_file));
        add("Файл отсутствует", testcase(this, "FileNotFound",
                                         &TestFileHandler::test_file_not_found));
        add("Запись данных", testcase(this, "WriteData",
                                      &TestFileHandler::test_write_data));
        add("Неверный формат файла", testcase(this, "InvalidFileFormat",
                                              &TestFileHandler::test_invalid_file_format));
    }

    void test_read_correct_file() {
        FileHandler fh;
        std::vector<Vector> vectors = fh.read_vectors("test_vectors.bin");
        ASSERT_EQUALS(vectors.size(), 5);
    }

    void test_file_not_found() {
        FileHandler fh;
        ASSERT_THROW(fh.read_vectors("missing_file.bin"), std::runtime_error);
    }

    void test_write_data() {
        FileHandler fh;
        std::vector<uint16_t> results = {10, 20, 30};
        ASSERT_NO_THROW(fh.write_results("results.bin", results));
    }

    void test_invalid_file_format() {
        FileHandler fh;
        ASSERT_THROW(fh.read_vectors("invalid_file.bin"), std::runtime_error);
    }
};