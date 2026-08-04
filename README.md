# Тестовое задание для стажера по направлению *Разработчик C++*
## https://github.com/Ksenia-rgb/infotecs-traineeship-developer-cxx
`tecslog` - разработанная в ходе выполнения задания библиотека для записи сообщений в журнал с разными уровнями важности.

`demo-app` - приложение для проверки работы библиотеки
## Структура папок
- `tecslog` - папка, содержащая файлы библиотеки
  - `include` - заголовочные файлы библиотеки для подключения
  - `src` - файлы с реализацией библиотеки
- `demo-app` - папка, содержащая файлы приложения для проверки работы библиотеки
- `tests` - папка, содержащая файлы с Unit-тестами библиотеки

## Сборка проекта
Проект собирается с помощью cmake.
`CmakeLists.txt` - файл с общими целями сборки

**Команды для сборки cmake (запускаются из корня репозитория):**
- Сборка библиотеки как статической: `BUILD_SHARED_LIBS` по умолчанию имеет значение `OFF`
```
cmake -B build -S . --toolchain=cmake/gcc-toolchain.cmake
cmake -B build -S . --toolchain=cmake/gcc-toolchain.cmake -DBUILD_SHARED_LIBS=OFF
```

- Сборка библиотеки как динамической:
```
cmake -B build -S . --toolchain=cmake/gcc-toolchain.cmake -DBUILD_SHARED_LIBS=ON
```

**Поддерживаемые цели сборки make (запускаются из папки `build`):**
- Сборка библиотеки
```
make tecslog
```
- Сборка демонстрационного приложения
```
make demo-app
```
- Сборка всех тестов
```
make tests
```

**Запуски:**
- Запуск демонстрационного приложения с передачей аргументов ():
```
./bin/demo-app ../logs/monday.log INFO
```

- Запуск тестов для библиотеки через CTest (должны быть заранее собраны)
```
make test
```

- Запуск тестов для библиотеки с подробным выводом
```
make test-verbose
```

## Примеры использования библиотеки
### Базовое использование
```c++
#include <tecslog/tecslog.hpp>

int main() 
{
  tecslog::init("logs/monday.log", tecslog::Level::INFO);
  tecslog::info("Welcome to tecslog");
  tecslog::warning("Some warning message");
  tecslog::error("Some error message);
  
  tecslog::setLevel(tecslog::Level::DEBUG); // Set default log level to debug
  tecslog::info("This message should be displayed");

  tecslog::log(tecslog::Level::DEBUG, "Another debug displayed message");
  tecslog::log(tecslog::Level::ERROR, "Another error displayed message");
}
```

### Использование строковых представлений Level
```c++
#include <tecslog/tecslog.hpp>

int main() 
{
  tecslog::init("logs/monday.log", "INFO"); // Set correct str level

  tecslog::log("INFO", "Welcome to tecslog");
  tecslog::log("WARNING", Some warning message");
  tecslog::error("ERROR", "Some error message);
  
  std::error_code code = tecslog::setLevel("CRITICAL"); // Set incorrect default level and get error_code != 0
  if (code)
  {
    std::cerr << code.message() << '\n'
    return 1;
  }

  code = tecslog::log("WARN", "Incorrect warn message"); // Specify incorrect log level and get error_code != 0
  if (code)
  {
    std::cerr << code.message() << '\n'
    return 1;
  }

  tecslog::printPossibleLevels(std::cout); // Output: 0 INFO 1 WARNING 2 ERROR
}
```
### Использование без начальной инициализации
```c++
#include <tecslog/tecslog.hpp>

int main() 
{
  tecslog::info("Hello, everyone"); // Log in default file (tecslog.log) with default level specified as min level (INFO)
}
