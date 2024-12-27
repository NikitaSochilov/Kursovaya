class TestAuthenticator : public suite {
public:
    TestAuthenticator()
        : suite("TestAuthenticator", "Тесты для аутентификации клиента") {
        add("Генерация соли", testcase(this, "GenerateSalt",
                                       &TestAuthenticator::test_generate_salt));
        add("Формат соли", testcase(this, "SaltFormat",
                                    &TestAuthenticator::test_salt_format));
        add("Создание сообщения", testcase(this, "GenerateAuthMessage",
                                           &TestAuthenticator::test_generate_auth_message));
        add("Отсутствие ошибок формата", testcase(this, "AuthMessageFormat",
                                                  &TestAuthenticator::test_auth_message_format));
        add("Обработка неверного пароля", testcase(this, "HandleWrongPassword",
                                                   &TestAuthenticator::test_handle_wrong_password));
        add("Обработка пустого логина", testcase(this, "HandleEmptyLogin",
                                                 &TestAuthenticator::test_handle_empty_login));
    }

    void test_generate_salt() {
        Authenticator auth("testuser", "password123");
        ASSERT_NO_THROW(auth.generate_salt());
    }

    void test_salt_format() {
        Authenticator auth("testuser", "password123");
        std::string salt = auth.generate_salt();
        ASSERT_EQUALS(salt.size(), 16);
        for (char c : salt) {
            ASSERT_TRUE(std::isxdigit(c));
        }
    }

    void test_generate_auth_message() {
        Authenticator auth("testuser", "password123");
        std::string message = auth.authenticate();
        ASSERT_TRUE(!message.empty());
    }

    void test_auth_message_format() {
        Authenticator auth("testuser", "password123");
        std::string message = auth.authenticate();
        ASSERT_TRUE(message.find("testuser") != std::string::npos);
    }

    void test_handle_wrong_password() {
        Authenticator auth("testuser", "wrongpassword");
        ASSERT_NO_THROW(auth.authenticate());
    }

    void test_handle_empty_login() {
        Authenticator auth("", "password123");
        ASSERT_THROW(auth.authenticate(), std::invalid_argument);
    }
};