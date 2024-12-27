#include "unit++.h"
#include "ConnectionTests.h"
#include "AuthenticatorTests.h"
#include "FileHandlerTests.h"
#include "DataHandlerTests.h"

int main() {
    // Создаём тестовый раннер
    unitpp::runner testrunner;

    // Подключаем тестовые наборы
    testrunner.add(new TestConnection());      // Тесты для Connection
    testrunner.add(new TestAuthenticator());   // Тесты для Authenticator
    testrunner.add(new TestFileHandler());     // Тесты для FileHandler
    testrunner.add(new TestDataHandler());     // Тесты для DataHandler

    // Запускаем все тесты
    bool test_result = testrunner.run();

    // Возвращаем результат выполнения тестов (0 или 1)
    return test_result ? 0 : 1;
}