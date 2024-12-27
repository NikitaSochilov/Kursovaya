class TestDataHandler : public suite {
public:
    TestDataHandler()
        : suite("TestDataHandler", "Тесты для обработки данных") {
        add("Отправка и результат", testcase(this, "SendAndReceiveSuccess",
                                             &TestDataHandler::test_send_and_receive));
        add("Множественные операции", testcase(this, "MultipleOperations",
                                               &TestDataHandler::test_multiple_operations));
    }

    void test_send_and_receive() {
        DataHandler dh;
        ASSERT_NO_THROW(dh.send_vector({1, 2, 3}));
    }

    void test_multiple_operations() {
        DataHandler dh;
        std::vector<std::vector<uint16_t>> vectors = {{1, 2, 3}, {4, 5, 6}};
        for (const auto& vec : vectors) {
            ASSERT_NO_THROW(dh.send_vector(vec));
        }
    }
};