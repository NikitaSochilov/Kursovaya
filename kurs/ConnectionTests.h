class TestConnection : public suite {
public:
    TestConnection()
        : suite("TestConnection", "Тесты для создания TCP-соединений") {
        add("Успешное соединение", testcase(this, "SuccessfulConnection",
                                            &TestConnection::test_successful_connection));
        add("Неверный адрес", testcase(this, "InvalidAddress",
                                       &TestConnection::test_invalid_address));
        add("Закрытый порт", testcase(this, "ClosedPort",
                                      &TestConnection::test_closed_port));
        add("Пустые параметры", testcase(this, "EmptyParameters",
                                         &TestConnection::test_empty_parameters));
    }

    void test_successful_connection() {
        Connection conn;
        ASSERT_NO_THROW(conn.establish("127.0.0.1", 33333));
        conn.close();
    }

    void test_invalid_address() {
        Connection conn;
        ASSERT_THROW(conn.establish("256.256.256.256", 33333), std::runtime_error);
    }

    void test_closed_port() {
        Connection conn;
        ASSERT_THROW(conn.establish("127.0.0.1", 12345), std::runtime_error);
    }

    void test_empty_parameters() {
        Connection conn;
        ASSERT_THROW(conn.establish("", 0), std::invalid_argument);
    }
};