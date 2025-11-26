#include <UnitTest++/UnitTest++.h>
#include "interface.h"

SUITE(HelpTest){
    TEST(ShortHelp){
        UserInterface iface;
        const char* argv[] = {"test","-h", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(!iface.parse_arguments(argc, argv));
        CHECK(!iface.get_help_text().empty());
    }
    
    TEST(Help){
        UserInterface iface;
        const char* argv[] = {"test","--help", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(!iface.parse_arguments(argc, argv));
        CHECK(!iface.get_help_text().empty());
    }
}

SUITE(ParamTest){
    TEST(CheckParsedValues){
        UserInterface iface;
        const char* argv[] = {"test", 
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));
    }

    TEST(InputFileWithAllRequiredParams){
        UserInterface iface;
        const char* argv[] = {"test", 
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));
    }
}

SUITE(ErrorTest){
    TEST(MissingRequiredParams){
        UserInterface iface;
        const char* argv[] = {"test","-i", "input.txt", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.parse_arguments(argc, argv), std::exception);
    }

    TEST(MissingInputFile){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));
    }

    TEST(MissingPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.parse_arguments(argc, argv), std::exception);
    }

    TEST(MissingAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.parse_arguments(argc, argv), std::exception);
    }

    TEST(InvalidPortType){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "not_a_number",  // не число
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.parse_arguments(argc, argv), std::exception);
    }

    TEST(UnknownParameter){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             "--unknown", "value",  // неизвестный параметр
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.parse_arguments(argc, argv), std::exception);
    }

    TEST(DuplicateParameters){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input1.txt",
                             "-i", "input2.txt",  // дубликат параметра
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.parse_arguments(argc, argv), std::exception);
    }

    TEST(MissingParameterValue){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i",  // отсутствует значение
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.parse_arguments(argc, argv), std::exception);
    }
}

SUITE(BoundaryTest){
    TEST(MinValidPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "1",  // минимальный валидный порт
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));
    }

    TEST(MaxValidPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "65535",  // максимальный валидный порт
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));
    }

    TEST(LocalhostAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "localhost",  // localhost как адрес
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));
    }

    // Тесты, которые проходят (парсер не проверяет семантику)
    TEST(InvalidPortNumber){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "99999",  // невалидный порт (но парсер принимает)
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));  // Ожидаем, что парсится без ошибок
    }

    TEST(NegativePort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "-1",  // отрицательный порт (но парсер принимает)
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));  // Ожидаем, что парсится без ошибок
    }

    TEST(InvalidAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "999.999.999.999",  // невалидный IP (но парсер принимает)
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));  // Ожидаем, что парсится без ошибок
    }

    TEST(EmptyFilename){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "",  // пустое имя файла (но парсер принимает)
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.parse_arguments(argc, argv));  // Ожидаем, что парсится без ошибок
    }
}

int main(){
    return UnitTest::RunAllTests();
}